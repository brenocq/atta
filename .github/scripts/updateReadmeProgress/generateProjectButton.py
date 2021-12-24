from github import Github
from google.cloud import storage
import cv2
import numpy as np
import os

# Gcloud
storage_client = storage.Client()
bucket = storage_client.get_bucket('atta-images')

# Github
import os
g = Github(os.environ['GITHUB_TOKEN'])
repo = g.get_repo("brenocq/atta")

# Update gcloud button images 
for project in repo.get_projects():
    try:
        columns = project.get_columns()
        # Calculate how much % of the project was done
        notDone = 0
        done = 0
        for column in project.get_columns():
            numCards = column.get_cards().totalCount
            if column.name == 'Done':
                done = done + numCards
            else:
                notDone = notDone + numCards

        progress = 1.0
        if notDone+done > 0:
            progress = done/float(notDone+done)
        print(project.name+' {:.2f}%'.format(progress*100))

        nameSnakeCase = project.name.replace(' ', '_').lower()
        filename = nameSnakeCase+'_button_github.png'

        # Download button base image
        blob = bucket.blob('main/'+filename)
        blob.download_to_filename(filename)

        # Add progress bar to base image
        img = cv2.imread(filename, cv2.IMREAD_UNCHANGED)
        height, width, channels = img.shape

        bgRadius = 77
        thickness = 3
        radius = bgRadius-thickness//2
        center = (23+bgRadius, 20+bgRadius)
        axes = (radius, radius)
        angle = 270
        startAngle = 0
        endAngle = 360*progress
        color = (70, 163, 58, 255)
        cv2.ellipse(img, center, axes, angle, startAngle, endAngle, color, thickness)

        # Update gcloud image
        progressFilename = filename.replace('.png', '_progress.png')
        cv2.imwrite(progressFilename, img)

        blob = bucket.blob('main/'+progressFilename)
        blob.upload_from_filename(progressFilename)

        # Delete temp files
        if os.path.exists(filename):
            os.remove(filename)
        if os.path.exists(progressFilename):
            os.remove(progressFilename)

    except Exception as e:
        print(e)
