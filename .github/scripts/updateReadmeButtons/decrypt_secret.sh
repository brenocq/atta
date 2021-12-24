#!/bin/sh

# Decrypt the file
mkdir $HOME/secrets
gpg --quiet --batch --yes --decrypt --passphrase="$SECRET_GCLOUD" \
--output $HOME/secrets/secret.json secret.json.gpg
