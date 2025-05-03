import logging
from datetime import datetime
import common
import github_data
from github_data import IssueType, IssueState

# Icons extracted from https://primer.style/octicons/
ISSUE_OPEN_16 = "M8 9.5a1.5 1.5 0 1 0 0-3 1.5 1.5 0 0 0 0 3Z M8 0a8 8 0 1 1 0 16A8 8 0 0 1 8 0ZM1.5 8a6.5 6.5 0 1 0 13 0 6.5 6.5 0 0 0-13 0Z"
ISSUE_CLOSED_16 = "M11.28 6.78a.75.75 0 0 0-1.06-1.06L7.25 8.69 5.78 7.22a.75.75 0 0 0-1.06 1.06l2 2a.75.75 0 0 0 1.06 0l3.5-3.5Z M16 8A8 8 0 1 1 0 8a8 8 0 0 1 16 0Zm-1.5 0a6.5 6.5 0 1 0-13 0 6.5 6.5 0 0 0 13 0Z"
BUG_16 = "M4.72.22a.75.75 0 0 1 1.06 0l1 .999a3.488 3.488 0 0 1 2.441 0l.999-1a.748.748 0 0 1 1.265.332.75.75 0 0 1-.205.729l-.775.776c.616.63.995 1.493.995 2.444v.327c0 .1-.009.197-.025.292.408.14.764.392 1.029.722l1.968-.787a.75.75 0 0 1 .556 1.392L13 7.258V9h2.25a.75.75 0 0 1 0 1.5H13v.5c0 .409-.049.806-.141 1.186l2.17.868a.75.75 0 0 1-.557 1.392l-2.184-.873A4.997 4.997 0 0 1 8 16a4.997 4.997 0 0 1-4.288-2.427l-2.183.873a.75.75 0 0 1-.558-1.392l2.17-.868A5.036 5.036 0 0 1 3 11v-.5H.75a.75.75 0 0 1 0-1.5H3V7.258L.971 6.446a.75.75 0 0 1 .558-1.392l1.967.787c.265-.33.62-.583 1.03-.722a1.677 1.677 0 0 1-.026-.292V4.5c0-.951.38-1.814.995-2.444L4.72 1.28a.75.75 0 0 1 0-1.06Zm.53 6.28a.75.75 0 0 0-.75.75V11a3.5 3.5 0 1 0 7 0V7.25a.75.75 0 0 0-.75-.75ZM6.173 5h3.654A.172.172 0 0 0 10 4.827V4.5a2 2 0 1 0-4 0v.327c0 .096.077.173.173.173Z"
PIN_16 = "m11.294.984 3.722 3.722a1.75 1.75 0 0 1-.504 2.826l-1.327.613a3.089 3.089 0 0 0-1.707 2.084l-.584 2.454c-.317 1.332-1.972 1.8-2.94.832L5.75 11.311 1.78 15.28a.749.749 0 1 1-1.06-1.06l3.969-3.97-2.204-2.204c-.968-.968-.5-2.623.832-2.94l2.454-.584a3.08 3.08 0 0 0 2.084-1.707l.613-1.327a1.75 1.75 0 0 1 2.826-.504ZM6.283 9.723l2.732 2.731a.25.25 0 0 0 .42-.119l.584-2.454a4.586 4.586 0 0 1 2.537-3.098l1.328-.613a.25.25 0 0 0 .072-.404l-3.722-3.722a.25.25 0 0 0-.404.072l-.613 1.328a4.584 4.584 0 0 1-3.098 2.537l-2.454.584a.25.25 0 0 0-.119.42l2.731 2.732Z"

ISSUE_COUNT = 10

logging.basicConfig(level=logging.DEBUG, format='[%(levelname)s] %(message)s')

def generate_issue_svg(issue):
    width = 800
    height = 120

    card_pad = 8 # Card padding
    int_pad = 4 # Internal padding

    # Issue icon
    issue_icon_size = 16
    issue_icon_path = ISSUE_OPEN_16
    if issue.type == IssueType.FEATURE and issue.state == IssueState.CLOSED:
        issue_icon_path = ISSUE_CLOSED_16
    elif issue.type == IssueType.BUG:
        issue_icon_path = BUG_16
    # Issue icon color
    issue_icon_class = "icon-open"
    if issue.state == IssueState.CLOSED:
        issue_icon_class = "icon-closed"

    # Format dates
    created_at_dt = datetime.strptime(issue.created_at, "%Y-%m-%dT%H:%M:%SZ")
    created_at_formatted = f"{created_at_dt.strftime('%b')} {created_at_dt.day}"
    last_commented_at_formatted = None
    if issue.last_commented_at:
        last_commented_at_dt = datetime.strptime(issue.last_commented_at, "%Y-%m-%dT%H:%M:%SZ")
        last_commented_at_formatted = f"{last_commented_at_dt.strftime('%b')} {last_commented_at_dt.day}"

    # Build the contributor text
    contributor_comment =  f'<tspan style="text-decoration: underline;">{issue.author}</tspan> opened on {created_at_formatted}'
    if issue.last_commenter and last_commented_at_formatted:
        contributor_comment = contributor_comment + f' · <tspan style="text-decoration: underline;">{issue.last_commenter}</tspan> commented on {last_commented_at_formatted}'

    svg = f"""
    <svg width="{width}" height="{height}" xmlns="http://www.w3.org/2000/svg">
        <!-- {issue.url} -->
        {common.STYLE}

        <!-- Card Background -->
        <rect class="card" x="0" y="0" width="{width}" height="{height}"/>

        <!-- Issue Icon -->
        <path class="{issue_icon_class}" transform="translate({card_pad}, {card_pad})" d="{issue_icon_path}"/>

        <!-- Issue Title -->
        <text class="title" x="{card_pad+issue_icon_size+int_pad}" y="{card_pad+issue_icon_size/2+2}" text-anchor="start" dominant-baseline="middle">
            {issue.title} <tspan style="fill: var(--fgColor-muted);font-weight: var(--text-weight-normal);">#{issue.number}</tspan>
        </text>

        """
    if issue.is_pinned:
        svg += f"""
        <!-- Pinned Icon -->
        <path class="icon-default" transform="translate({width-card_pad-issue_icon_size}, {card_pad})" d="{PIN_16}"/>

        """

    label_x = card_pad
    label_y = card_pad + issue_icon_size + int_pad
    for label in issue.labels:
        label_color = label.color
        label_text = label.name
        label_width = common.estimate_text_width(label_text, 12) + 15
        label_height = 16

        svg += f"""
        <!-- Label -->
        <g transform="translate({label_x + label_width/2}, {label_y})">
            <rect class="labelBg-muted" x="{-label_width/2}" y="0" width="{label_width}" height="{label_height}" rx="8"/>
            <text class="label-muted" x="0" y="{label_height/2+1}" text-anchor="middle" dominant-baseline="middle">{label_text}</text>
        </g>
        """
        label_x += label_width + int_pad

    right_footer_x = width - card_pad
    if issue.deletions is not None:
        right_footer_x -= common.estimate_text_width(f"-{issue.deletions}", 14)
        svg += f"""
            <!-- Deletions -->
            <text class="small" fill="var(--fgColor-danger)" x="{right_footer_x}" y="{height-card_pad-1}">-{issue.deletions}</text>
            """
    if issue.additions is not None:
        right_footer_x -= common.estimate_text_width(f"-{issue.additions}", 14) + int_pad*2
        svg += f"""
            <!-- Additions -->
            <text class="small" fill="var(--fgColor-success)" x="{right_footer_x}" y="{height-card_pad-1}">+{issue.additions}</text>
            """


    svg += f"""
        <!-- Contributor Comment -->
        <text class="small" fill="var(--fgColor-muted)" x="{card_pad}" y="{height-card_pad-1}">{contributor_comment}</text>
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
