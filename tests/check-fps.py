#!/usr/bin/env python3
"""Compare both FPS fields with independent oracles and check real templates."""
from pathlib import Path
import argparse,json,os,resource,subprocess

parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--source',type=Path,required=True)
parser.add_argument('--acl',type=Path,required=True)
parser.add_argument('--output',type=Path,required=True)
parser.add_argument('--templates',action='store_true')
parser.add_argument('--sanitizers',action='store_true')
args=parser.parse_args()
source=args.source.resolve();acl=args.acl.resolve();output=args.output.resolve()
output.mkdir(parents=True,exist_ok=True)
resource.setrlimit(resource.RLIMIT_CORE,(0,0))
tests=Path(__file__).resolve().parent
extensions=['bostan_mori.cpp','multipoint.cpp','polynomial_product.cpp','combinatorial_fps.cpp']
templates=['minimal']
if args.templates:
    templates=['template.cpp','Templates/multiple_test_cases.cpp','Templates/codeforces.cpp']
invalid=['negative_prefix','negative_shift','inverse_zero','log_constant','exp_constant','negative_power',
         'divide_zero','polynomial_zero','duplicate_points','point_count','recurrence_count','negative_partition']
results=[]
for core in ['fps.cpp','fps_mod1000000007.cpp']:
    library=''.join((source/'FPSandPolynomial'/name).read_text()+'\n' for name in [core,*extensions])
    for template in templates:
        if template=='minimal':
            prefix='#include <bits/stdc++.h>\n#include <atcoder/convolution>\nusing namespace std;using namespace atcoder;using ll=long long;\n'
        else:
            prefix='#include <atcoder/convolution>\n'+(source/template).read_text().split('signed main(){',1)[0]
        modes=['normal','ndebug']+(['sanitized'] if args.sanitizers and template==templates[0] else [])
        for mode in modes:
            label=Path(core).stem+'_'+Path(template).stem+'_'+mode
            cpp=output/(label+'.cpp');exe=output/label
            cpp.write_text(prefix+'\n'+library+(tests/'fps-regression.hpp').read_text())
            flags=['-DNDEBUG'] if mode=='ndebug' else []
            if mode=='sanitized':flags=['-fsanitize=address,undefined','-fno-sanitize-recover=all']
            subprocess.run(['g++','-std=gnu++23','-O2','-I'+str(acl),*flags,str(cpp),'-o',str(exe)],check=True)
            env=os.environ.copy();env['ASAN_OPTIONS']='detect_leaks=0'
            ran=subprocess.run([str(exe)],env=env,text=True,capture_output=True,timeout=120)
            if ran.returncode:raise SystemExit(label+': '+ran.stdout+ran.stderr)
            for case in invalid:
                failed=subprocess.run([str(exe),case],env=env,text=True,capture_output=True,timeout=10)
                if failed.returncode!=-6 or failed.stdout or failed.stderr:
                    raise SystemExit('Expected silent SIGABRT: '+label+' '+case+' '+str((failed.returncode,failed.stdout,failed.stderr[:300])))
            result={'core':core,'template':template,'mode':mode,'result':ran.stdout.strip(),'silent_invalid_cases':len(invalid)}
            results.append(result)
            print(label,ran.stdout.strip(),flush=True)
(output/'results.json').write_text(json.dumps(results,ensure_ascii=False,indent=2)+'\n')
print('All '+str(len(results))+' configurations passed',flush=True)
