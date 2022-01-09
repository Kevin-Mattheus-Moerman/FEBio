#! /bin/bash
set -o errexit
set -o verbose

. ./common-functions.sh

MMG_SOURCE=https://github.com/MmgTools/mmg/archive/master.zip
MMG_ARCHIVE=$(basename $MMG_SOURCE)
MMG_PATH="mmg-${MMG_ARCHIVE%.*}"

build_and_install() {
	local source=$1
	pushd "$source" || exit 1
	mkdir build
	pushd build || exit 1
	cmake ..
	make -j "$(nproc)"
	make install
	popd || exit 1
	popd || exit 1
}

main() {
	pushd "$BUILD_PATH" || exit 1
	download_source "$MMG_SOURCE"
	extract_source "$MMG_ARCHIVE"
	build_and_install "$MMG_PATH"
	popd || exit 1
}

main
