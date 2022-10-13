#!/bin/bash
FILE=/etc/hosts
SITE1=www.site1.com
SITE2=www.site2.com
SITE3=www.site3.com

# coloca
sudo chmod 666 /etc/hosts
#if grep -E $SITE $FILE
#then
#	echo "Tirando host ..."
#	sudo sed -i "" "/$SITE/d" /etc/hosts
#	echo "ok"
#fi
echo "$(grep -v $SITE1 $FILE)" > $FILE
echo "$(grep -v $SITE2 $FILE)" > $FILE
echo "$(grep -v $SITE3 $FILE)" > $FILE

sudo chmod 644 /etc/hosts
