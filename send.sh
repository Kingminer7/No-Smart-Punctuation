#!/usr/bin/env bash
set -e

# SERVER="http://192.168.4.57:8080"
SERVER="http://10.50.37.109:8080"

shopt -s nullglob
FILES=(build/*.geode)

if [ ${#FILES[@]} -eq 0 ]; then
  echo "No .geode files found in build/"
  exit 1
fi

echo "Uploading ${#FILES[@]} file(s)..."

CURL_ARGS=()
for f in "${FILES[@]}"; do
  CURL_ARGS+=(-F "files=@${f}")
done

curl -X POST "${SERVER}/upload" \
  "${CURL_ARGS[@]}"

echo
echo "Done."

curl -X POST "${SERVER}/launch"