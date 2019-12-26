#!/bin/bash
FALSE=0
TRUE=1
OTHERIN=2

checkInput()
{
    local YESNO
    YESNO=$1

    if [ "$YESNO" == "y" -o \
         "$YESNO" == "yes" -o \
         "$YESNO" == "Y" -o \
         "$YESNO" == "YES" -o \
         "$YESNO" == "Yes" ]; then
        return $TRUE;
    elif [ "$yesno" == "n" -o \
           "$yesno" == "no" -o \
           "$yesno" == "N" -o \
           "$yesno" == "NO" -o \
           "$yesno" == "No" ]; then
        return $FALSE;
    else
        return $OTHERIN;
    fi
}

while :
do
    echo ""
    read -p "Do you set proxy server ? [y/n] " yesno
    checkInput $yesno
    yesnorc=$?
    if [ $yesnorc == $TRUE ]; then
        read -p "Please input your proxy server: " proxyserver
        read -p "Please input your proxy port number: " proxyport
        if [ "$proxyserver" == "" ]; then
            echo ""
            echo "Please retry."
            continue;
        fi

        echo ""
        echo "Your Proxy Server: $proxyserver"
        echo "Your Proxy Port: $proxyport"
        echo ""
        read -p "Is it OK ? [y/n] " isok
        checkInput $isok
        isokrc=$?
        if [ $isokrc == $TRUE ]; then
            if [ "$proxyport" == "" ]; then
                echo "Set into /etc/environment"
                sudo sh -c "echo 'http_proxy=\"http://$proxyserver/\"' >> /etc/environment"
                sudo sh -c "echo 'https_proxy=\"http://$proxyserver/\"' >> /etc/environment"
                echo "Set into /etc/apt/apt.conf"
                sudo sh -c "echo 'Acquire::http::Proxy \"http://$proxyserver/\";' >> /etc/apt/apt.conf"
            else
                echo "Set into /etc/environment"
                sudo sh -c "echo 'http_proxy=\"http://$proxyserver:$proxyport/\"' >> /etc/environment"
                sudo sh -c "echo 'https_proxy=\"http://$proxyserver:$proxyport/\"' >> /etc/environment"
                echo "Set into /etc/apt/apt.conf"
                sudo sh -c "echo 'Acquire::http::Proxy \"http://$proxyserver:$proxyport/\";' >> /etc/apt/apt.conf"
            fi
            break;
        else
            echo ""
            echo "Please retry."
            continue;
        fi
    elif [ $yesnorc == $FALSE ]; then
        break
    else
        continue;
    fi
done
