#! /bin/sh

REFBIN=assignment/linux64/TextMiningApp
REFDICT=build/ref.dict
MYBIN=assignment/linux64/TextMiningApp
MYDICT=build/ref.dict

if [ $# -lt 1 ]
then
    echo "Usage: $0 testfile"
    exit 1
fi

until [ $# -eq 0 ]; do
    TESTFILE=$1
    TESTFILENAME=$(basename $TESTFILE)
    REFOUTFILE=build/tests/output/$TESTFILENAME.ref.out
    MYOUTFILE=build/tests/output/$TESTFILENAME.my.out
    echo "================ $TESTFILENAME ================"
    echo -n "Ref time: ";
    (time -f"time: %e" $REFBIN $REFDICT < $TESTFILE > $REFOUTFILE) 2>&1 | grep time
    echo -n "My time: ";
    (time -f"time: %e" $MYBIN $REFDICT < $TESTFILE > $MYOUTFILE) 2>&1 | grep time
    if diff $REFOUTFILE $MYOUTFILE; then
        echo PASS
    else
        echo FAIL
    fi
    shift
done
