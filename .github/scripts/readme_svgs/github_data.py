import os
import requests
import logging
import json
from dataclasses import dataclass, field
from enum import Enum, auto
from typing import List, Dict, Optional, Any

#----- Configuration -----#
REPO_OWNER = "brenocq"
REPO_NAME = "atta"
GITHUB_TOKEN = os.environ.get('GITHUB_TOKEN')
API_BASE_URL = "https://api.github.com/graphql"
if not GITHUB_TOKEN:
    raise ValueError("GITHUB_TOKEN environment variable is not set")

HEADERS = {
    "Authorization": f"Bearer {GITHUB_TOKEN}",
    "Content-Type": "application/json"
}

#----- Logging -----#
logging.basicConfig(level=logging.DEBUG, format='[%(levelname)s] %(message)s')

#----- Data Classes -----#
class IssueType(Enum):
    BUG = auto()
    FEATURE = auto()
    OTHER = auto()

class IssueState(Enum):
    OPEN = auto()
    CLOSED = auto()

class PrState(Enum):
    OPEN = auto()
    CLOSED = auto()
    MERGED = auto()

class IssueInteraction(Enum):
    OPENED = auto()
    EDITED = auto()
    COMMENTED = auto()

@dataclass
class Label:
    name: str
    color: str # Hex color with # prefix

@dataclass
class Reactions:
    plus_one: int = 0
    minus_one: int = 0
    laugh: int = 0
    hooray: int = 0
    confused: int = 0
    heart: int = 0
    rocket: int = 0
    eyes: int = 0

@dataclass
class Issue:
    number: int
    title: str # Without the [Feature] or [Bug] prefix
    type: IssueType
    state: IssueState
    url: str
    author: str
    labels: List[Label]
    contributors: List[str]
    contributors_avatars: List[str]
    reactions: Reactions
    last_interaction_user: str
    last_interaction_type: IssueInteraction
    last_interaction_at: str # ISO 8601 format string
    is_pinned: bool = False
    completed_tasks: int = 0
    total_tasks: int = 0
    comment_count: int = 0
    linked_branch: Optional[str] = None
    linked_pr: Optional[int] = None
    linked_pr_state: Optional[PrState] = None
    commit_count: Optional[int] = None
    additions: Optional[int] = None
    deletions: Optional[int] = None

#----- Parsing Helper -----#
def parse_issue_from_data(issue_data: Dict[str, Any]) -> Issue:
    """Parses raw API dict into an Issue dataclass, requires post-processing."""

    #--- Basic Fields ---#
    state_enum = IssueState.OPEN if issue_data.get('state') == 'OPEN' else IssueState.CLOSED
    comment_count = issue_data.get('comments', {}).get('totalCount', 0)
    author_login = issue_data.get('author', {}).get('login', 'unknown')
    author_avatar = issue_data.get('author', {}).get('avatarUrl', '')
    updated_at = issue_data.get('updated_at', '')

    #--- Issue title & type ---#
    title = issue_data.get('title', '')
    issue_type = IssueType.OTHER
    if title.startswith("[Feature]"):
        issue_type = IssueType.FEATURE
        title = title.replace("[Feature]", "").strip()
    elif title.startswith("[Bug]"):
        issue_type = IssueType.BUG
        title = title.replace("[Bug]", "").strip()

    #--- Labels ---#
    parsed_labels = [
        Label(name=lbl['name'], color=f"#{lbl['color']}")
        for lbl in issue_data.get('labels', {}).get('nodes', [])
    ]

    #--- Reactions ---#
    counts = {
        "THUMBS_UP": 0,
        "THUMBS_DOWN": 0,
        "LAUGH": 0,
        "HOORAY": 0,
        "CONFUSED": 0,
        "HEART": 0,
        "ROCKET": 0,
        "EYES": 0,
    }
    raw_reactions = issue_data.get('reactionGroups', [])
    for raw_rection in raw_reactions:
        counts[raw_rection['content']] = raw_rection['reactors']['totalCount']
    parsed_reactions = Reactions(
        plus_one=counts.get('THUMBS_UP', 0),
        minus_one=counts.get('THUMBS_DOWN', 0),
        laugh=counts.get('LAUGH', 0),
        hooray=counts.get('HOORAY', 0),
        confused=counts.get('CONFUSED', 0),
        heart=counts.get('HEART', 0),
        rocket=counts.get('ROCKET', 0),
        eyes=counts.get('EYES', 0),
    )

    #--- Contributors ---#
    # Author
    contributors_map = {author_login: author_avatar}
    # Assignees
    assignees = issue_data.get('assignees').get('nodes', [])
    for assignee in assignees:
        login = assignee.get('login')
        if login not in contributors_map:
            contributors_map[login] = assignee.get('avatarUrl')
    # Commenters
    comments = issue_data.get('comments').get('nodes', [])
    for comment in comments:
        commenter = comment.get('author')
        login = commenter.get('login')
        if login not in contributors_map:
            contributors_map[login] = commenter.get('avatarUrl')
    # Insertion order is preserved in Python 3.7+
    contributors_logins = list(contributors_map.keys())
    contributors_avatars = list(contributors_map.values())

    #--- Linked branch ---#
    linked_branches_data = issue_data.get('linkedBranches', {}).get('nodes', [])
    linked_branch_name = None
    linked_branch_commit = None
    # If there are multiple linked branches, we take the first one
    for branch in linked_branches_data:
        branch_ref = branch.get('ref', {})
        linked_branch_name = branch_ref.get('name')
        linked_branch_commit = branch_ref.get('target', {}).get('oid')
        break

    #--- Linked PR ---#
    timeline_items = issue_data.get('timelineItems', {}).get('nodes', [])
    linked_pr = None
    linked_pr_state = None
    for item in timeline_items:
        subject = item.get('subject')
        linked_pr = subject.get('number')
        linked_pr_state = PrState.OPEN
        if subject.get('state') == 'MERGED':
            linked_pr_state = PrState.MERGED
        elif subject.get('state') == 'CLOSED':
            linked_pr_state = PrState.CLOSED

    #--- Parse tasks ---#
    body = issue_data.get('body', '')
    completed_tasks = body.count("[x]") + body.count("\\[x\\]") + body.count("[X]") + body.count("\\[X\\]")
    incomplete_tasks = body.count("[ ]") + body.count("\\[ \\]")
    total_tasks = completed_tasks + incomplete_tasks

    #--- TODO commits/additions/deletions ---#

    #--- TODO Last interaction ---#

    #--- Create Issue ---#
    issue = Issue(
        number = issue_data['number'],
        title = title,
        type = issue_type,
        is_pinned = issue_data.get('isPinned', False),
        state = state_enum,
        url = issue_data['url'],
        author = author_login,
        comment_count = comment_count,
        labels = parsed_labels,
        contributors = contributors_logins,
        contributors_avatars = contributors_avatars,
        reactions = parsed_reactions,
        completed_tasks = completed_tasks,
        total_tasks = total_tasks,
        linked_branch = linked_branch_name,
        linked_pr = linked_pr,
        linked_pr_state = linked_pr_state,
        last_interaction_user = author_login, # TODO
        last_interaction_type = IssueInteraction.OPENED, # TODO
        last_interaction_at = updated_at, # TODO
    )

    return issue

#----- Fetch Issues -----#
def fetch_issues(owner: str, repo: str, count: int) -> List[Issue]:
    """
    Fetches issues, prioritizing pinned issues, then filling with recent open issues.

    Args:
        owner: Repository owner.
        repo: Repository name.
        count: The total number of issues desired in the final list.

    Returns:
        A list of Issue objects, with pinned issues first, up to 'count' total.
    """

    query = f"""
    query GetIssues {{
      repository(owner: "{owner}", name: "{repo}") {{
        issues(first: 10, orderBy: {{field: UPDATED_AT, direction: DESC}}) {{
          nodes {{
              number
              title
              isPinned
              url
              state
              createdAt
              updatedAt
              author {{ login avatarUrl(size: 40) }}
              labels(first: 10) {{ nodes {{ name color }} }}
              assignees(first: 10) {{ nodes {{ login avatarUrl(size: 40) }} }}
              comments(first: 50) {{ totalCount nodes {{ author {{ login avatarUrl(size: 40) }} }} }}
              reactionGroups {{
                content
                reactors {{
                  totalCount
                }}
              }}
              # Get linked branches
              linkedBranches(first: 10) {{
                nodes {{
                  ref {{
                    name
                    target {{
                      ... on Commit {{
                        oid
                      }}
                    }}
                  }}
                }}
              }}
              # Get linked PRs
              timelineItems(itemTypes: [CONNECTED_EVENT], first: 10) {{
                nodes {{
                  ... on ConnectedEvent {{
                    subject {{
                      ... on PullRequest {{
                        number
                        title
                        url
                        state
                        merged
                        author {{
                          login
                        }}
                      }}
                    }}
                  }}
                }}
              }}
              # Get body (parse for tasks)
              body
            }}
          }}
        }}
      }}
    """

    fetched_issues: List[Issue] = []

    logging.info(f"Fetching last {count} updated open issues from {owner}/{repo}...")
    try:
        response = requests.post(API_BASE_URL, headers=HEADERS, json={"query": query})
        response.raise_for_status()
        data = response.json()
        issues_data = data.get('data', {}).get('repository', {}).get('issues', {}).get('nodes', [])

        if not issues_data:
            logging.info("No open issues found matching the criteria.")
            return fetched_issues

        logging.info(f"Received {len(issues_data)} items from API. Parsing...")

        for issue_data in issues_data:
            try:
                # Parse the raw data into the structured Issue object
                issue = parse_issue_from_data(issue_data)
                fetched_issues.append(issue)
            except Exception as parse_error:
                 logging.error(f"Failed to parse issue #{issue_data.get('number')}: {parse_error}", exc_info=True)


    except requests.exceptions.RequestException as e:
        logging.error(f"Error fetching issues from GitHub API: {e}")
    except json.JSONDecodeError as e:
         logging.error(f"Error decoding JSON response from GitHub API: {e}")
    except Exception as e:
        logging.error(f"An unexpected error occurred: {e}")

    logging.info(f"Returning {len(fetched_issues)} parsed issues.")
    return fetched_issues

#----- Main -----#
if __name__ == "__main__":
    top_issues = fetch_issues(REPO_OWNER, REPO_NAME, 5)
    for issue in top_issues:
        logging.info(10*"-" + f"ISSUE #{issue.number}" + 10*"-");
        logging.info(f"PRINT {issue}");
