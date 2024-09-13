# !/bin/bash

function utils_check_ret()
{
	if [ $1 -eq 0 ]; then
        echo "[INFO] $2 done!"
    else
        echo "[ERR] Failed on $2!"
        exit -1
    fi
}

BRANCH=v1.8.0
if [[ "$UNAME_STR" == "Linux" ]]; then
    echo "Linux OS"
    git clone https://github.com/stlink-org/stlink.git --depth=1 -b ${BRANCH} || \
    utils_check_ret $? "clone failed!"
    pushd stlink
    cmake . && \
    make -j8 && \
    sudo cp bin/st-* /usr/local/bin/ && \
    sudo cp lib/*.so* /lib32
    popd
elif [[ "$UNAME_STR" == *"MINGW"* || "$UNAME_STR" == *"CYGWIN"* ]]; then
    echo "Windows OS"
elif [[ "$UNAME_STR" == "Darwin" ]]; then
    echo "MAC OS"
else
    echo "Unsupported OS: $UNAME_STR"
    exit 1
fi


st-flash -h

rm -rf stlink