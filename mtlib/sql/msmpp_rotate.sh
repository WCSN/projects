#!/bin/sh

HOST_SQL="localhost"
USER_SQL="root"
PASS_SQL=""
DB_SQL_1="msmpp"
DB_SQL_2="msmpp"


###############################################

DATE_OLD1=$(date -dnow-1day +%d%m%Y)
DATE_OLD2=$(date -dnow-2day +%d%m%Y)
DATE_OLD3=$(date -dnow-3day +%d%m%Y)
DATE_OLD4=$(date -dnow-4day +%d%m%Y)
DATE_OLD5=$(date -dnow-5day +%d%m%Y)
DATE_NOW=$(date +%d%m%Y)

SQLTABLE_NOW_tMsgIds="
CREATE TABLE tMsgIds_"$DATE_NOW" (
  msg_id char(65) NOT NULL,
  req_id int(11) default NULL,
  PRIMARY KEY  (msg_id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
"

SQLTABLE_NOW_tDeliverSmCache="
CREATE TABLE tDeliverSmCache_"$DATE_NOW" (
  req_id int(11) NOT NULL,
  context char(255) default NULL,
  part_num int(11) default NULL,
  PRIMARY KEY  (req_id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
"


############### for msmpp

res=`echo "$SQLTABLE_NOW_tMsgIds" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`
res=`echo "$SQLTABLE_NOW_tDeliverSmCache" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`
echo $res >> /var/log/rotate_create.log
##############################################
ROTATEREQUEST="alter table tMsgIds TYPE=MERGE UNION=(tMsgIds_$DATE_NOW,tMsgIds_$DATE_OLD1,tMsgIds_$DATE_OLD2,tMsgIds_$DATE_OLD3
) INSERT_METHOD=FIRST;"
`echo "$ROTATEREQUEST" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`

ROTATEREQUEST2="alter table tDeliverSmCache TYPE=MERGE UNION=(tDeliverSmCache_$DATE_NOW,tDeliverSmCache_$DATE_OLD1,tDeliverSmCa
che_$DATE_OLD2,tDeliverSmCache_$DATE_OLD3) INSERT_METHOD=FIRST;"
res=`echo "$ROTATEREQUEST2" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`
echo $res >> /var/log/rotate_create.log


##############################################
DROPREQUEST="drop table tMsgIds_$DATE_OLD4"
`echo "$DROPREQUEST" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`

DROPREQUEST="drop table tDeliverSmCache_$DATE_OLD4"
`echo "$DROPREQUEST" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`

#################################  end of db msmpp
