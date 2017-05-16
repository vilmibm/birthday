# birthday.c

being a simple C program for listing users' birthdays on a social unix server.

# installation and use

    $ echo "10/11" > /home/vilmibm/.birthday
    $ # clone this repo
    $ gcc birthday.c > birthday
    $ ./birthday 10/11
    vilmibm
    $ date -u +"%m/%d" | xargs ./birthday # check for birthdays today
    

# unasked questions

## why is this in C

because the author is brushing up on C skills

## where would i ever run this

probably not many places, but definitely on [tilde.town](https://tilde.town).

## why is this licensed under the GPL

because the GPL loves you

# author

[~vilmibm](https://tilde.town/~vilmibm)

# license

GPLv3
