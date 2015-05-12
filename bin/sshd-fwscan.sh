#!/bin/sh
if ipfw show | awk '{print $1}' | grep -q 20000 ; then
        ipfw delete 20000
fi
# This catches repeated attempts for both legal and illegal users
# No check for duplicate entries is performed, since the rule
# has been deleted.
awk '/sshd/ && (/Invalid user/ || /authentication error/) {try[$(NF)]++}
END {for (h in try) if (try[h] > 5) print h}' /var/log/auth.log |
while read ip
do
        ipfw -q add 20000 deny tcp from $ip to any in
done
