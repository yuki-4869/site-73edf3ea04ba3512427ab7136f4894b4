#!/usr/bin/env python3
"""Verify large FPS identities, interpolation and combinatorial sequences."""
from pathlib import Path
import argparse,json,resource,subprocess

parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--source',type=Path,required=True)
parser.add_argument('--acl',type=Path,required=True)
parser.add_argument('--output',type=Path,required=True)
args=parser.parse_args()
source=args.source.resolve();acl=args.acl.resolve();output=args.output.resolve()
output.mkdir(parents=True,exist_ok=True)
resource.setrlimit(resource.RLIMIT_CORE,(0,0))
tests=Path(__file__).resolve().parent
prefix='#include <bits/stdc++.h>\n#include <atcoder/convolution>\nusing namespace std;using namespace atcoder;using ll=long long;\n'
extensions=['bostan_mori.cpp','multipoint.cpp','polynomial_product.cpp','combinatorial_fps.cpp']
results=[]
for core in ['fps.cpp','fps_mod1000000007.cpp']:
    code=prefix+''.join((source/'FPSandPolynomial'/name).read_text()+'\n' for name in [core,*extensions])+(tests/'fps-large.hpp').read_text()
    cpp=output/(Path(core).stem+'.cpp');exe=cpp.with_suffix('')
    cpp.write_text(code)
    subprocess.run(['g++','-std=gnu++23','-O2','-DNDEBUG','-I'+str(acl),str(cpp),'-o',str(exe)],check=True)
    ran=subprocess.run([str(exe)],capture_output=True,text=True,timeout=120)
    if ran.returncode:raise SystemExit(core+': '+ran.stdout+ran.stderr)
    results.append({'core':core,'result':ran.stdout.strip()})
    print(ran.stdout.strip(),flush=True)
(output/'results.json').write_text(json.dumps(results,ensure_ascii=False,indent=2)+'\n')
