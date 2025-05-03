import logging
from datetime import datetime
from common import STYLE, estimate_text_size
import github_data
from github_data import IssueType, IssueState

# Icons extracted from https://primer.style/octicons/
ISSUE_OPEN_24 = "M12 1c6.075 0 11 4.925 11 11s-4.925 11-11 11S1 18.075 1 12 5.925 1 12 1ZM2.5 12a9.5 9.5 0 0 0 9.5 9.5 9.5 9.5 0 0 0 9.5-9.5A9.5 9.5 0 0 0 12 2.5 9.5 9.5 0 0 0 2.5 12Zm9.5 2a2 2 0 1 1-.001-3.999A2 2 0 0 1 12 14Z"
ISSUE_CLOSED_24 = "M17.28 9.28a.75.75 0 0 0-1.06-1.06l-5.97 5.97-2.47-2.47a.75.75 0 0 0-1.06 1.06l3 3a.75.75 0 0 0 1.06 0l6.5-6.5Z M12 1c6.075 0 11 4.925 11 11s-4.925 11-11 11S1 18.075 1 12 5.925 1 12 1ZM2.5 12a9.5 9.5 0 0 0 9.5 9.5 9.5 9.5 0 0 0 9.5-9.5A9.5 9.5 0 0 0 12 2.5 9.5 9.5 0 0 0 2.5 12Z"
BUG_24 = "M7.72.22a.75.75 0 0 1 1.06 0l1.204 1.203A4.98 4.98 0 0 1 12 1c.717 0 1.4.151 2.016.423L15.22.22a.751.751 0 0 1 1.042.018.751.751 0 0 1 .018 1.042l-.971.972A4.991 4.991 0 0 1 17 6v1.104a2.755 2.755 0 0 1 1.917 1.974l1.998-.999a.75.75 0 0 1 .67 1.342L19 10.714V13.5l3.25.003a.75.75 0 0 1 0 1.5L19 15.001V16c0 .568-.068 1.134-.204 1.686l.04.018 2.75 1.375a.75.75 0 1 1-.671 1.342l-2.638-1.319A6.998 6.998 0 0 1 12 23a6.998 6.998 0 0 1-6.197-3.742l-2.758 1.181a.752.752 0 0 1-1.064-.776.752.752 0 0 1 .474-.602l2.795-1.199A6.976 6.976 0 0 1 5 16v-.996H1.75a.75.75 0 0 1 0-1.5H5v-2.79L2.415 9.42a.75.75 0 0 1 .67-1.342l1.998.999A2.756 2.756 0 0 1 7 7.104V6a4.99 4.99 0 0 1 1.69-3.748l-.97-.972a.75.75 0 0 1 0-1.06ZM6.5 9.75V16a5.5 5.5 0 1 0 11 0V9.75c0-.69-.56-1.25-1.25-1.25h-8.5c-.69 0-1.25.56-1.25 1.25ZM8.5 7h7V6a3.5 3.5 0 1 0-7 0Z"

ISSUE_COUNT = 10

logging.basicConfig(level=logging.DEBUG, format='[%(levelname)s] %(message)s')

def generate_issue_svg(issue):
    width = 800
    height = 120

    # Issue icon
    issue_icon_path = ISSUE_OPEN_24
    if issue.type == IssueType.FEATURE and issue.state == IssueState.CLOSED:
        issue_icon_path = ISSUE_CLOSED_24
    elif issue.type == IssueType.BUG:
        issue_icon_path = BUG_24
    # Issue icon color
    issue_icon_class = "open"
    if issue.state == IssueState.CLOSED:
        issue_icon_class = "closed"

    svg = f"""
    <svg width="{width}" height="{height}" xmlns="http://www.w3.org/2000/svg">
        {STYLE}
        <!-- Card Background -->
        <rect class="card" x="0" y="0" width="{width}" height="{height}"/>

        <!-- Issue Icon -->
        <path class="{issue_icon_class}" transform="translate(5, 5)" d="{issue_icon_path}"/>
    </svg>
    """
    return svg

if __name__ == "__main__":
    issues = github_data.fetch_top_issues(ISSUE_COUNT)
    for i, issue in enumerate(issues):
        logging.info(f"Processing issue: {issue.title}")
        svg = generate_issue_svg(issue)

        # Save each SVG to a unique file
        filename = f"issue_{i}.svg"
        with open(filename, "w") as f:
            f.write(svg)
        logging.info(f"Saved SVG as {filename}")
