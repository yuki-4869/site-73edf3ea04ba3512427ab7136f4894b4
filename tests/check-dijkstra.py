#!/usr/bin/env python3
"""Exercise the exact shortest-path implementations in all basic templates."""
import argparse
import json
from pathlib import Path
import subprocess

parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--source',type=Path,required=True)
parser.add_argument('--acl',type=Path,required=True)
parser.add_argument('--output',type=Path,required=True)
args=parser.parse_args()
source=args.source.resolve()
acl=args.acl.resolve()
output=args.output.resolve()
output.mkdir(parents=True,exist_ok=True)
harness=Path(__file__).with_name('dijkstra_test.cpp').read_text()
print_harness=Path(__file__).with_name('template_print_test.cpp').read_text()
policy=json.loads(Path(__file__).resolve().parents[1].joinpath('template-policy.json').read_text())
paths=['template.cpp','Templates/multiple_test_cases.cpp','Templates/codeforces.cpp','Templates/modint_only.cpp']
for path in paths:
    full=(source/path).read_text()
    lines=full.splitlines()
    positions=[i for i,line in enumerate(lines) if line.startswith('vl dijkstra(')]
    assert len(positions)==6 and positions==list(range(positions[0],positions[0]+6)),path
    assert any(line.startswith('const vl Pow2={') for line in lines),path
    first=next(i for i,line in enumerate(lines) if line.startswith('vl vl_n('))
    assert lines[first+1].startswith('vl vl_N('),path
    if path=='template.cpp':
        begin,end=policy['line_positions']['dijkstra']
        assert positions==list(range(begin-1,end)),path
        assert lines[policy['line_positions']['Pow2']-1].startswith('const vl Pow2={'),path
    text=full.split('signed main(){',1)[0]
    cpp=output/(Path(path).stem+'.cpp')
    executable=cpp.with_suffix('')
    cpp.write_text(text+'\n'+harness+'\n'+print_harness+'\nint main(){dijkstra_test_all();template_print_test_all();}\n')
    subprocess.run(['g++','-std=c++23','-O2','-fsanitize=undefined','-fno-sanitize-recover=undefined',
                    '-I'+str(acl),str(cpp),'-o',str(executable)],check=True)
    print(path,flush=True)
    subprocess.run([str(executable)],check=True)
