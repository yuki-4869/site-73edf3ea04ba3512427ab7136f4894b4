import fs from "node:fs/promises";
import path from "node:path";
import assert from "node:assert/strict";
import {execFileSync} from "node:child_process";
const original=path.resolve(process.env.SOURCE_DIR||"source");
const temp=await fs.mkdtemp(path.join(process.cwd(),".sync-test-"));
const source=path.join(temp,"source");
const output=path.join(temp,"output");
try {
 await fs.cp(original,source,{recursive:true,filter:p=>path.basename(p)!==".git"});
 const manifest=JSON.parse(await fs.readFile("dist/assets/source-manifest.json","utf8"));
 assert(manifest.files.length>=2,"Need at least two sources for the synchronization fixture");
 const changed=manifest.files[0].path,removed=manifest.files[1].path;
 await fs.appendFile(path.join(source,changed),"\n// Synchronization test fixture.\n");
 await fs.unlink(path.join(source,removed));
 await fs.mkdir(path.join(source,"SyncFixture"),{recursive:true});
 await fs.writeFile(path.join(source,"SyncFixture/new_source.cpp"),"int sync_fixture() { return 42; }\n");
 const env={...process.env,SOURCE_DIR:source,OUTPUT_DIR:output,SOURCE_SHA:"1".repeat(40)};
 execFileSync(process.execPath,["scripts/build.mjs"],{env,stdio:"pipe"});
 execFileSync(process.execPath,["scripts/check.mjs"],{env,stdio:"pipe"});
 const search=JSON.parse(await fs.readFile(path.join(output,"assets/search.json"),"utf8"));
 const added=search.find(x=>x.path==="SyncFixture/new_source.cpp");
 assert(added,"New source must appear in search");
 assert(!search.some(x=>x.path===removed),"Deleted source must disappear from search");
 const removedOutput=path.join(output,"library",removed.replace(/\.cpp$/,".html"));
 await assert.rejects(fs.access(removedOutput),"Deleted source page must be removed");
 const changedHtml=await fs.readFile(path.join(output,"library",changed.replace(/\.cpp$/,".html")),"utf8");
 assert(changedHtml.includes("公開コードが更新されています"),"Changed source must mark old documentation");
 assert(changedHtml.includes("/blob/"+"1".repeat(40)+"/"),"Reference links must follow the new source revision");
 const addedHtml=await fs.readFile(path.join(output,added.url),"utf8");
 assert(addedHtml.includes("API・制約・計算量の説明は追記予定"),"New sources must have an honest fallback description");
 const after=JSON.parse(await fs.readFile(path.join(output,"assets/source-manifest.json"),"utf8"));
 assert.equal(after.files.length,manifest.files.length,"Adding and removing one file must preserve the source count");
 console.log("Synchronization scenarios passed: add, change, remove, stale-document notice, new permalink, source parity.");
} finally {
 await fs.rm(temp,{recursive:true,force:true});
}
