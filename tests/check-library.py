#!/usr/bin/env python3
"""Check paste compatibility and executable usage examples against the sources."""
from pathlib import Path
import argparse
import concurrent.futures
import hashlib
import json
import re
import resource
import subprocess

PARSER = argparse.ArgumentParser(description=__doc__)
PARSER.add_argument('--source', type=Path, required=True)
PARSER.add_argument('--acl', type=Path, required=True)
PARSER.add_argument('--output', type=Path, required=True)
PARSER.add_argument('--jobs', type=int, default=4)
ARGS = PARSER.parse_args()
SOURCE = ARGS.source.resolve()
ACL = ARGS.acl.resolve()
OUT = ARGS.output.resolve()
OUT.mkdir(parents=True,exist_ok=True)
SITE = Path(__file__).resolve().parents[1]
TEMPLATES = {'code':'template.cpp','codet':'Templates/multiple_test_cases.cpp','codecf':'Templates/codeforces.cpp'}
HEADERS = {
    'DataStructureSegment/segtree_wrappers.cpp':['atcoder/segtree'],
    'DataStructureSegment/lazy_segtree_wrappers.cpp':['atcoder/lazysegtree'],
    'Snippets/segtree_custom.cpp':['atcoder/segtree'],
    'Snippets/lazy_segtree_custom.cpp':['atcoder/lazysegtree'],
}
resource.setrlimit(resource.RLIMIT_CORE,(0,0))


def raw(path):
    return (SOURCE/path).read_text(encoding='utf-8-sig')


def implementation(path):
    text=raw(path)
    if path in TEMPLATES.values():
        return text.rstrip()+'\n'
    return text.split('/*',1)[0].rstrip()+'\n'


def strip_cpp_comments(text):
    tokens=re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|//[^\n]*|/\*[\s\S]*?\*/')
    return tokens.sub(lambda m:'\n'*m[0].count('\n') if m[0].startswith(('//','/*')) else m[0],text)


def common(template):
    return implementation(TEMPLATES[template]).split('signed main(){',1)[0]


def basic(path):
    if path in TEMPLATES.values():
        return documented_example(path)
    return raw(path).split('\n基本の関数\n',1)[1].rsplit('*/',1)[0].strip()


def documented_example(path):
    page=SITE/'content/usage'/Path(path).with_suffix('.md')
    section=page.read_text().split('## 使用例\n',1)[1]
    return re.search(r'```cpp\n(.*?)\n```',section,re.S)[1]


def policy(path):
    text=raw(path)
    if path in TEMPLATES.values():
        link='// https://github.com/yuki-4869/Algorithm/blob/main/'+path+'\n'
        assert text.startswith(link), path
    else:
        assert text.count('/*')==text.count('*/')==1, path
        assert text.endswith('*/\n'), path
        assert text.index('細かい注意点')<text.index('使える関数一覧')<text.index('基本の関数'),path
    body=implementation(path)
    if path in TEMPLATES.values():
        body=strip_cpp_comments(body)
    else:
        assert '//' not in body and '/*' not in body, path
    assert not re.search(r'\b(?:throw|assert)\s*\(?',body),path
    assert not re.search(r'\b(?:cerr|clog|fprintf|printf|puts|perror)\b',body),path
    if path not in TEMPLATES.values():
        assert not re.search(r'\n\s*\n\s*\n',body),path
    if path not in TEMPLATES.values() and path!='Templates/modint_only.cpp':
        assert not re.search(r'^\s*#\s*include',body,re.M),path


def translation(path,template,example=False):
    prefix=common(template)
    if template=='codecf':
        prefix=''.join('#include <'+h+'>\n' for h in HEADERS.get(path,[]))+prefix
    text=implementation(path)
    code=basic(path)
    if example:
        code=documented_example(path)
    if path=='template.cpp' or path.startswith('Templates/'):
        return implementation(path).split('signed main(){',1)[0]+'\nint main(){\n'+code+'\n}\n'
    if path=='Tree/virtual_tree.cpp' and example:
        text=implementation('Tree/hld.cpp')+'\n'+text
    if path=='Snippets/binary_search.cpp':
        text=''
    if path=='String/palindrome_radius.cpp':
        if example:
            text=''
        else:
            text=text.replace('string S;','string S="abacaba";')
            code='auto radius=R[3]; auto length=2*radius-1; assert(radius==4 && length==7);'
    if path in ['Snippets/segtree_custom.cpp','Snippets/lazy_segtree_custom.cpp'] and example:
        text=''
    elif path=='Snippets/lazy_segtree_custom.cpp':
        text=implementation('DataStructureSegment/lazy_segtree_wrappers.cpp')+'\n'+text
    inside = path=='String/palindrome_radius.cpp' and not example
    if inside:
        return prefix+'\nint main(){\n'+text+'\n'+code+'\n}\n'
    return prefix+'\n'+text+'\n'+(code if re.search(r'\bint main\s*\(',code) else 'int main(){\n'+code+'\n}\n')


def check(task):
    path,template,example=task
    label=re.sub(r'[^A-Za-z0-9_]','_',path)+'_'+template+('_example' if example else '_basic')
    cpp=OUT/(label+'.cpp')
    executable=OUT/label
    cpp.write_text(translation(path,template,example))
    command=['g++','-std=c++23','-O0','-I'+str(ACL),str(cpp),'-o',str(executable)]
    try:
        built=subprocess.run(command,capture_output=True,text=True,timeout=120)
        (OUT/(label+'.log')).write_text(built.stderr)
        if built.returncode:
            return {'path':path,'template':template,'example':example,'status':'compile_failed','error':built.stderr[:2200]}
        ran=subprocess.run([str(executable)],capture_output=True,text=True,timeout=15)
        return {'path':path,'template':template,'example':example,'status':'pass' if ran.returncode==0 else 'run_failed','stderr':ran.stderr[:1200]}
    except subprocess.TimeoutExpired:
        return {'path':path,'template':template,'example':example,'status':'timeout'}


paths=sorted(str(p.relative_to(SOURCE)) for p in SOURCE.rglob('*.cpp'))
for path in paths: policy(path)
tasks=[(p,t,False) for p in paths for t in TEMPLATES if not (p=='template.cpp' or p.startswith('Templates/')) or t=='code']
tasks += [(p,'code',True) for p in paths]
results=[]
with concurrent.futures.ThreadPoolExecutor(max_workers=ARGS.jobs) as pool:
    for result in pool.map(check,tasks):
        results.append(result)
        if result['status']!='pass': print(json.dumps(result,ensure_ascii=False),flush=True)
        elif len(results)%20==0: print(f'Passed {len(results)}/{len(tasks)} compilation/run checks',flush=True)
failures=[r for r in results if r['status']!='pass']
report={'files':len(paths),'checks':len(results),'passed':len(results)-len(failures),'failures':failures,'additional_headers_for_codecf':HEADERS,'source_sha256':{p:hashlib.sha256((SOURCE/p).read_bytes()).hexdigest() for p in paths}}
(OUT/'results.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='source_sha256'},ensure_ascii=False),flush=True)
raise SystemExit(bool(failures))
