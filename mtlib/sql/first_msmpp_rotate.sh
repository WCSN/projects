#!/bin/sh

HOST_SQL="localhost"
USER_SQL="root"
PASS_SQL=""
DB_SQL_1="msmpp"
###############################################

DATE_OLD1=$(date -dnow-1day +%d%m%Y)
DATE_OLD2=$(date -dnow-2day +%d%m%Y)
DATE_OLD3=$(date -dnow-3day +%d%m%Y)
DATE_OLD4=$(date -dnow-4day +%d%m%Y)
DATE_OLD5=$(date -dnow-5day +%d%m%Y)
DATE_NOW=$(date +%d%m%Y)

SQLTABLE_tMsgIds="
CREATE TABLE tMsgIds (
  msg_id char(65) NOT NULL,
  req_id int(11) default NULL,
  PRIMARY KEY  (msg_id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
"

SQLTABLE_tDeliverSmCache="
CREATE TABLE tDeliverSmCache (
  req_id int(11) NOT NULL,
  context char(255) default NULL,
  part_num int(11) default NULL,
  PRIMARY KEY  (req_id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
"


SQLTABLE_DATE_OLD1_tMsgIds="
CREATE TABLE tMsgIds_"$DATE_OLD1" (
  msg_id char(65) NOT NULL,
  req_id int(11) default NULL,
  PRIMARY KEY  (msg_id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
"

SQLTABLE_DATE_OLD1_tDeliverSmCache="
CREATE TABLE tDeliverSmCache_"$DATE_OLD1" (
  req_id int(11) NOT NULL,
  context char(255) default NULL,
  part_num int(11) default NULL,
  PRIMARY KEY  (req_id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
"

SQLTABLE_DATE_OLD2_tMsgIds="
CREATE TABLE tMsgIds_"$DATE_OLD2" (
  msg_id char(65) NOT NULL,
  req_id int(11) default NULL,
  PRIMARY KEY  (msg_id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
"

SQLTABLE_DATE_OLD2_tDeliverSmCache="
CREATE TABLE tDeliverSmCache_"$DATE_OLD2" (
  req_id int(11) NOT NULL,
  context char(255) default NULL,
  part_num int(11) default NULL,
  PRIMARY KEY  (req_id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
"
SQLTABLE_DATE_OLD3_tMsgIds="
CREATE TABLE tMsgIds_"$DATE_OLD3" (
  msg_id char(65) NOT NULL,
  req_id int(11) default NULL,
  PRIMARY KEY  (msg_id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
"

SQLTABLE_DATE_OLD3_tDeliverSmCache="
CREATE TABLE tDeliverSmCache_"$DATE_OLD3" (
  req_id int(11) NOT NULL,
  context char(255) default NULL,
  part_num int(11) default NULL,
  PRIMARY KEY  (req_id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
"

###############

`echo "rename table tMsgIds to tMsgIds_$DATE_NOW" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`

`echo "rename table tDeliverSmCache to tDeliverSmCache_$DATE_NOW" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`




res=`echo "$SQLTABLE_tMsgIds" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`
res=`echo "$SQLTABLE_tDeliverSmCache" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`



res=`echo "$SQLTABLE_DATE_OLD1_tMsgIds" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`
res=`echo "$SQLTABLE_DATE_OLD1_tDeliverSmCache" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`

res=`echo "$SQLTABLE_DATE_OLD2_tMsgIds" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`
res=`echo "$SQLTABLE_DATE_OLD2_tDeliverSmCache" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`

res=`echo "$SQLTABLE_DATE_OLD3_tMsgIds" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`
res=`echo "$SQLTABLE_DATE_OLD3_tDeliverSmCache" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`


##############################################
ROTATEREQUEST="alter table tMsgIds TYPE=MERGE UNION=(tMsgIds_$DATE_NOW,tMsgIds_$DATE_OLD1,tMsgIds_$DATE_OLD2,tMsgIds_$DATE_OLD3) INSERT_METHOD=FIRST;"
#echo $ROTATEREQUEST
`echo "$ROTATEREQUEST" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`


ROTATEREQUEST2="alter table tDeliverSmCache TYPE=MERGE UNION=(tDeliverSmCache_$DATE_NOW,tDeliverSmCache_$DATE_OLD1,tDeliverSmCache_$DATE_OLD2,tDeliverSmCache_$DATE_OLD3) INSERT_METHOD=FIRST;"
echo $ROTATEREQUEST2

`echo "$ROTATEREQUEST2" | mysql --user=$USER_SQL --password=$PASS_SQL --host=$HOST_SQL --database=$DB_SQL_1`


##############################################

