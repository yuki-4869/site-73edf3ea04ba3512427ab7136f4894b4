import fs from "node:fs/promises";
import path from "node:path";
import assert from "node:assert/strict";
import {createHash} from "node:crypto";
const config=JSON.parse(await fs.readFile("site.config.json","utf8"));
const dist=path.resolve(process.env.OUTPUT_DIR||"dist");
const source=JSON.parse(await fs.readFile(path.join(dist,"assets/source-manifest.json"),"utf8"));
const build=JSON.parse(await fs.readFile(path.join(dist,"assets/build-manifest.json"),"utf8"));
const search=JSON.parse(await fs.readFile(path.join(dist,"assets/search.json"),"utf8"));
assert.equal(source.commit,build.sourceCommit);
assert.equal(source.files.length,build.codeCount);
assert.equal(search.length,build.pageCount);
const docs=[];
async function walk(dir){for(const e of await fs.readdir(dir,{withFileTypes:true})){const p=path.join(dir,e.name);if(e.isDirectory())await walk(p);else if(e.name.endsWith(".html"))docs.push(p);}}
await walk(dist);
const ids=new Map();
for(const p of docs){
 const html=await fs.readFile(p,"utf8");
 assert(!html.includes("@@"),"Unresolved template in "+p);
 assert(html.includes('name="robots" content="noindex,nofollow,noarchive"'),"Missing indexing directive");
 assert(!/href="https:\/\/github\.com\/yuki-4869\/Algorithm\/blob\/(?![a-f0-9]{40}\/)/.test(html),"Non-pinned source link: "+p);
 const found=Array.from(html.matchAll(/\bid="([^"]+)"/g),m=>m[1]);
 assert.equal(new Set(found).size,found.length,"Duplicate ids: "+p);
 ids.set(p,new Set(found));
}
const broken=[];
for(const p of docs){
 const html=await fs.readFile(p,"utf8");
 for(const match of html.matchAll(/\b(?:href|src)="([^"]+)"/g)){
  const url=match[1].replace(/&amp;/g,"&");
  if(/^(https?:|mailto:|data:)/.test(url))continue;
  const [raw,anchor]=url.split("#");
  let target;
  if(!raw)target=p;
  else if(raw.startsWith(config.siteBase))target=path.join(dist,decodeURIComponent(raw.slice(config.siteBase.length)));
  else if(raw.startsWith("/")){broken.push({page:p,url});continue;}
  else target=path.resolve(path.dirname(p),decodeURIComponent(raw));
  try{if((await fs.stat(target)).isDirectory())target=path.join(target,"index.html");await fs.access(target);}
  catch{broken.push({page:p,url});continue;}
  if(anchor&&target.endsWith(".html")&&!ids.get(target)?.has(decodeURIComponent(anchor)))broken.push({page:p,url,reason:"Missing fragment"});
 }
}
assert.deepEqual(broken,[],"Broken internal links");
for(const item of source.files){
 const bytes=await fs.readFile(path.join(dist,"sources",item.path));
 assert.equal(createHash("sha256").update(bytes).digest("hex"),item.sha256,"Source mismatch: "+item.path);
 assert.equal(createHash("sha1").update("blob "+bytes.length+"\0").update(bytes).digest("hex"),item.blob);
 assert(item.url.includes("/blob/"+source.commit+"/"),"Source reference must be pinned");
 const itemSearch=search.find(x=>x.path===item.path);assert(itemSearch,"Missing search entry");
 const html=await fs.readFile(path.join(dist,itemSearch.url),"utf8");
 assert(html.includes('id="full-source"'),"Missing full source");
 assert(html.includes('data-copy-url="'+item.url+'"'),"Wrong reference copy URL");
}
const index=await fs.readFile(path.join(dist,"index.html"),"utf8");
assert.equal((index.match(/class="library-row"/g)||[]).length,search.length);
console.log(JSON.stringify({htmlPages:docs.length,codeFiles:source.files.length,searchEntries:search.length,internalLinks:"pass",sourceHashes:"pass",pinnedReferences:"pass"}));
