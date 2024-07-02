set echo off

echo " building boobs"

pushd engine > /dev/null

. build.sh
popd > /dev/null

ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]; then
    echo "   error: $ERRORLEVEL" && exit
fi

echo "  built engine"

pushd testbed > /dev/null
. build.sh
popd > /dev/null

ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]; then
    echo "   error: $ERRORLEVEL" && exit
fi

echo "  built testbed"

echo " built boobs"