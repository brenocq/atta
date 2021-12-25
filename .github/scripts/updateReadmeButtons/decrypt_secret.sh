#!/bin/sh

# Decrypt the file
gpg --quiet --batch --yes --decrypt --passphrase="$SECRET_GCLOUD" \
--output secret.json secret.json.gpg
echo `pwd`/secret.json
