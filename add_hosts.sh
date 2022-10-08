#!/usr/bin/bash
set +x
FILE=/etc/hosts
SITE1=www.site1.com
SITE2=www.site2.com
SITE3=www.site3.com


# coloca
sudo chmod 666 /etc/hosts
if grep -E $SITE1 $FILE
then
		echo "/etc/hosts ok"
	else
			echo "adding sites to /etc/hosts"
				echo "127.0.0.1 "$SITE1 >> $FILE
				echo "127.0.0.1 "$SITE2 >> $FILE
				echo "127.0.0.1 "$SITE3 >> $FILE
fi
sudo chmod 644 /etc/hosts
