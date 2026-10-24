#!/usr/bin/env python3
"""Exercise the exact shortest-path implementations in all basic templates."""
import argparse
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
paths=['template.cpp','Templates/multiple_test_cases.cpp','Templates/codeforces.cpp','Templates/modint_only.cpp']
for path in paths:
    text=(source/path).read_text().split('signed main(){',1)[0]
    cpp=output/(Path(path).stem+'.cpp')
    executable=cpp.with_suffix('')
    cpp.write_text(text+'\n'+harness+'\nint main(){dijkstra_test_all();}\n')
    subprocess.run(['g++','-std=c++23','-O2','-fsanitize=undefined','-fno-sanitize-recover=undefined',
                    '-I'+str(acl),str(cpp),'-o',str(executable)],check=True)
    print(path,flush=True)
    subprocess.run([str(executable)],check=True)
