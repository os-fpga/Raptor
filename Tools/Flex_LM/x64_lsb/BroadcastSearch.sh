#! /bin/sh

FOLDER1=$HOME/.hasplm
FILE1=$FOLDER1/hasp_61320.ini

FOLDER2=/etc/hasplm
FILE2=$FOLDER2/hasplm.ini

if [ ! -d "$FOLDER1" ] || [ ! -d "$FOLDER2" ]; then
        mkdir -p $FOLDER1
        echo 'broadcastsearch = 0' > $FILE1
        sudo mkdir -p $FOLDER2
        sudo bash -c "echo 'broadcastsearch = 0' > $FILE2"
		echo "Configuration Successful!"
		
elif [ ! -f "$FILE1" ] || [ ! -f "$FILE2" ]; then
        echo 'broadcastsearch = 0' > $FILE1
        sudo bash -c "echo 'broadcastsearch = 0' > $FILE2"
		echo "Configuration Successful!"
else
        if grep -q "broadcastsearch = 1" $FILE1 ||  grep -q "broadcastsearch = 1" $FILE2 ; then
			sed -i 's/broadcastsearch = 1/broadcastsearch = 0/1' $FILE1
			sudo sed -i 's/broadcastsearch = 1/broadcastsearch = 0/1' $FILE2
			echo "Configuration Successful!"

        elif  ! grep -q "broadcastsearch = 0" $FILE1  ||  ! grep -q "broadcastsearch = 0" $FILE2 ; then
              
			echo 'broadcastsearch = 0' > $FILE1
			sudo bash -c "echo 'broadcastsearch = 0' > $FILE2"
			
			echo "Configuration Successful!"
                
        else
                echo 'Configuration not required!'
        fi
fi
