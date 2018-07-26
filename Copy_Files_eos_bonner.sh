#!/usr/bin/expect -f

set timeout 86400
set pass "XXXXXXX"
set depth [list 3 4 5 6 7 8]
set trees [list 50 100 200 300 400 500 600 700 800]
################
spawn sh
expect "$ "
send "export EOS_MGM_URL=root://eoscms.cern.ch\r"
expect "$ "

for { set i 0 } { $i < [llength $depth] } { incr i } {

set DEPTH [lindex $depth $i]

for { set j 0 } { $j < [llength $trees] } { incr j } {

set TREES [lindex $trees $j]

send "scp ws13@bonner02.rice.edu:/home/ws13/TMVA/TMVA/EMTFPtAssign2017/pTMulticlass_MODE_15_bitCompr_RPC_25_depth_$DEPTH\_trees_$TREES\.root /eos/user/w/wshi/Binary_Classifier_Optimize\r"
expect "ws13@bonner02.rice.edu's password: "
send "$pass\r"
expect "$ "
sleep 2
}
}

send "exit\r"
expect "$ "
