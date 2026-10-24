#!/usr/bin/env python3
"""Verify that every literal snippet expands to the exact corresponding source."""
import argparse
import json
from pathlib import Path
import re
import subprocess

parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--source',type=Path,required=True)
parser.add_argument('--snippets',type=Path,required=True)
parser.add_argument('--baseline',type=Path)
args=parser.parse_args()
site=Path(__file__).resolve().parents[1]
source=args.source.resolve()
snippets=json.loads(args.snippets.read_text(encoding='utf-8-sig'))
prefixes=json.loads((site/'snippet-prefixes.json').read_text())
paths=set(subprocess.check_output(['git','-C',str(source),'ls-files','-z'],text=True).split('\0'))
paths={p for p in paths if p.endswith('.cpp')}
revision=subprocess.check_output(['git','-C',str(source),'rev-parse','HEAD'],text=True).strip()
seen=set()
for name,entry in snippets.items():
    _,path,sha=entry['description'].split(' | ')
    assert path in paths and path not in seen,(name,path)
    assert sha==revision[:7],(path,sha)
    assert entry['scope']=='cpp',path
    encoded='\n'.join(entry['body'])
    expanded=[]
    i=0
    while i<len(encoded):
        c=encoded[i]
        if c=='\\':
            assert i+1<len(encoded) and encoded[i+1] in '\\$}',(path,i)
            expanded.append(encoded[i+1])
            i+=2
        else:
            assert c not in '$}',(path,i)
            expanded.append(c)
            i+=1
    assert ''.join(expanded).encode('utf-8')==(source/path).read_bytes(),path
    if path in prefixes:
        assert name==prefixes[path]['name'] and entry['prefix']==prefixes[path]['prefix'],path
    seen.add(path)
assert seen==paths,(seen^paths)
for name,path in [('コード','template.cpp'),('codet','Templates/multiple_test_cases.cpp'),('codecf','Templates/codeforces.cpp')]:
    assert snippets[name]['body'][0]=='// https://github.com/yuki-4869/Algorithm/blob/main/'+path,name
    assert snippets[name]['body'][-1]=='\t',name
    text=(source/path).read_text()
    if name=='コード':
        assert text.endswith('signed main(){\n    ios::sync_with_stdio(false); cin.tie(nullptr);\n\t'),name
    else:
        assert text.endswith('void solve(){\n\t'),name
if args.baseline:
    for name,entry in json.loads(args.baseline.read_text(encoding='utf-8-sig')).items():
        assert name in snippets and snippets[name]['prefix']==entry['prefix'],name
policy=json.loads((site/'template-policy.json').read_text())
shared=[]
for path in policy['shared_templates']:
    full=(source/path).read_text()
    comments={}
    for line in full.splitlines():
        if '//' in line and not line.lstrip().startswith('//'):
            code,comment=line.split('//',1)
            key=re.sub(r'\s+','',code.partition('{')[0].strip())
            comments[key]=comment.strip()
    assert comments==policy['inline_comments'],path
    marker='\nvoid solve();' if path.startswith('Templates/') else '\nsigned main(){'
    prefix=full.split(marker,1)[0]
    prefix=prefix[prefix.index('using namespace std;'):]
    tokens=re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|//[^\n]*|/\*[\s\S]*?\*/')
    prefix=tokens.sub(lambda m:'' if m[0].startswith(('//','/*')) else m[0],prefix)
    shared.append(re.sub(r'\s+','',prefix))
assert len(set(shared))==1,'Common declarations must follow code order in codet/codecf.'
cf=(source/'Templates/codeforces.cpp').read_text()
assert [line for line in cf.splitlines() if line.startswith('//#include <atcoder/')]==policy['codeforces_header_candidates']
assert policy['codeforces_expander_comment'] in cf
assert cf.index(policy['codeforces_expander_comment'])<cf.index('\nvoid solve();')
lines=(source/policy['line_reference']).read_text().splitlines()
assert all(line.startswith('vl dijkstra(') for line in lines[87:93])
assert lines[94].startswith('const vl Pow2={')
print(f'{len(seen)} literal snippets match the complete source bytes; names, prefixes, and template links verified.')
