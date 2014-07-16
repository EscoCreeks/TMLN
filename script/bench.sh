#! /bin/sh

REFBIN=../assignment/linux64/TextMiningApp
REFDICT=../build/ref.dict
MYBIN=../assignment/linux64/TextMiningApp
MYDICT=../build/ref.dict

if [ $# -ne 1 ]
then
    echo "Usage: $0 testfile"
    exit 1
fi

TESTFILE=$1
echo "================ REF $TESTFILE ================"
time -f"time: %e" $REFBIN $REFDICT < $TESTFILE
echo "================ MY $TESTFILE ================"
time -f"time: %e" $MYBIN $REFDICT < $TESTFILE
