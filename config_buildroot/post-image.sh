#!/bin/bash

set -e

GENIMAGE_TMP="${BUILD_DIR}/genimage.tmp"
CONFIG_DIR="$(dirname $0)"
GENIMAGE_CFG="${CONFIG_DIR}/genimage-three-digits.cfg"

cp "${CONFIG_DIR}/config.txt" "${BINARIES_DIR}/rpi-firmware"

trap 'rm -rf "${ROOTPATH_TMP}"' EXIT
ROOTPATH_TMP="$(mktemp -d)"

rm -rf "${GENIMAGE_TMP}"

genimage \
	--rootpath "${ROOTPATH_TMP}"   \
	--tmppath "${GENIMAGE_TMP}"    \
	--inputpath "${BINARIES_DIR}"  \
	--outputpath "${BINARIES_DIR}" \
	--config "${GENIMAGE_CFG}"

exit $?

