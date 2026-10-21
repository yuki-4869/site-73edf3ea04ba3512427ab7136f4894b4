#!/usr/bin/env python3
"""Generate literal VS Code snippets from a clean public Algorithm checkout."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

SITE=Path(__file__).resolve().parents[1]

def git(source,*args):
    return subprocess.check_output(['git','-C',str(source),*args],text=True).strip()

def escape(text):
    return text.replace('\\','\\\\').replace('$','\\$').replace('}','\\}')

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--baseline',type=Path,help='Also preserve every existing name and prefix from this file')
    args=parser.parse_args()
    source=args.source.resolve()
    if git(source,'status','--porcelain','--untracked-files=all'):
        raise SystemExit('Commit or discard source checkout changes before generating snippets.')
    revision=git(source,'rev-parse','HEAD')
    if not re.fullmatch(r'[0-9a-f]{40}',revision):
        raise SystemExit('Expected a complete source commit SHA.')
    paths=sorted(p for p in git(source,'ls-files','-z').split('\0') if p.endswith('.cpp'))
    catalog={item['path']:item for item in json.loads((SITE/'catalog.json').read_text())}
    prefixes=json.loads((SITE/'snippet-prefixes.json').read_text())
    snippets={}
    for path in paths:
        title=catalog.get(path,{}).get('title',Path(path).stem)
        definition=prefixes.get(path,{'name':title,'prefix':list(dict.fromkeys([title,Path(path).stem]))})
        name=definition['name']
        if name in snippets:
            raise SystemExit('Duplicate snippet name: '+name)
        text=(source/path).read_bytes().decode('utf-8')
        snippets[name]={
            'scope':'cpp',
            'prefix':definition['prefix'],
            'body':escape(text).split('\n'),
            'description':f'{title} | {path} | {revision[:7]}',
        }
    if args.baseline:
        baseline=json.loads(args.baseline.read_text(encoding='utf-8-sig'))
        for name,old in baseline.items():
            if name not in snippets or snippets[name]['prefix']!=old['prefix']:
                raise SystemExit('Existing snippet name/prefix changed: '+name)
    data=(json.dumps(snippets,ensure_ascii=False,indent=2)+'\n').encode('utf-8')
    args.output.parent.mkdir(parents=True,exist_ok=True)
    args.output.write_bytes(data)
    print(json.dumps({'file':str(args.output.resolve()),'snippets':len(snippets),
                      'source_commit':revision,'sha256':hashlib.sha256(data).hexdigest()},ensure_ascii=False))

if __name__=='__main__':
    main()
