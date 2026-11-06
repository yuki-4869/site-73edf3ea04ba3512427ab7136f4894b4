import fs from "node:fs/promises";
import path from "node:path";
import assert from "node:assert/strict";
import { JSDOM } from "jsdom";
import { Marked } from "marked";
import { renderProse } from "./document.mjs";
import { execFileSync } from "node:child_process";
import os from "node:os";

const dist = path.resolve(process.env.OUTPUT_DIR || "dist");
const config = JSON.parse(await fs.readFile("site.config.json", "utf8"));
const index = JSON.parse(await fs.readFile(path.join(dist,"assets/search.json"),"utf8"));
const app = await fs.readFile("web/app.js", "utf8");
const css = await fs.readFile("web/style.css", "utf8");
const tick = () => new Promise(resolve => setImmediate(resolve));
const snippetTemp=await fs.mkdtemp(path.join(os.tmpdir(),"algorithm-snippet-reference-"));
const actualSnippets=new Map();
try {
  const snippetFile=path.join(snippetTemp,"algorithm.code-snippets");
  execFileSync("python3",["scripts/make-snippets.py","--source",path.resolve(process.env.SOURCE_DIR||"source"),"--output",snippetFile],{stdio:"pipe"});
  for(const [name,entry] of Object.entries(JSON.parse(await fs.readFile(snippetFile,"utf8")))) {
    const sourcePath=entry.description.split(" | ")[1];
    actualSnippets.set(sourcePath,{name,prefixes:Array.isArray(entry.prefix)?entry.prefix:[entry.prefix]});
  }
} finally { await fs.rm(snippetTemp,{recursive:true,force:true}); }

async function page(file, hash = "") {
  const dom = new JSDOM(await fs.readFile(path.join(dist,file),"utf8"), {
    url:config.siteOrigin + config.siteBase + file + hash, runScripts:"outside-only", pretendToBeVisual:true
  });
  const style = dom.window.document.createElement("style"); style.textContent = css;
  dom.window.document.head.append(style);
  dom.window.fetch = async () => ({ok:true,json:async()=>index});
  dom.window.HTMLElement.prototype.scrollIntoView = () => {};
  const copied = [];
  Object.defineProperty(dom.window.navigator,"clipboard",{value:{writeText:async text=>copied.push(text)}});
  Object.defineProperty(dom.window,"isSecureContext",{value:true});
  dom.window.eval(app);
  return {dom,window:dom.window,document:dom.window.document,copied};
}

// The API and its examples must survive presentation changes on every page.
for (const entry of index) {
  const raw = await fs.readFile("content/usage/" + entry.path.replace(/\.(cpp|txt)$/,".md"),"utf8").catch(()=>null);
  if (!raw) continue;
  const original = raw.replace(/^# .*\r?\n/,"").replace(/^\[説明書の目次\].*\r?\n/gm,"");
  const snippets = [], oldIds = [];
  const used = new Set([...original.matchAll(/<a\s+id="([^"]+)"/g)].map(m=>m[1]));
  const markdown = new Marked({renderer:{
    heading(token) {
      const title = token.text.replace(/<[^>]*>/g," ").replace(/!?\[([^\]]+)\]\([^)]*\)/g,"$1").replace(/[\x60*#_]/g,"").replace(/\s+/g," ").trim();
      const slug = title.normalize("NFKC").toLowerCase().replace(/[^\p{L}\p{N}\s_-]/gu,"").trim().replace(/\s+/g,"-") || "section";
      let id = slug, n = 1; while (used.has(id)) id = slug + "-" + n++;
      used.add(id); oldIds.push(id); return "";
    },
    code(token) { snippets.push(token.text.replace(/\r\n?/g,"\n")); return ""; }
  }});
  markdown.parse(original);
  const dom = new JSDOM(await fs.readFile(path.join(dist,entry.url),"utf8"));
  const doc = dom.window.document;
  const trigger=doc.querySelector("#documentation .snippet-trigger");
  const actualSnippet=actualSnippets.get(entry.path);
  if(actualSnippet) {
    assert(trigger,`${entry.path}: missing snippet prefixes`);
    assert.deepEqual([...trigger.querySelectorAll("code")].map(code=>code.textContent),actualSnippet.prefixes,`${entry.path}: snippet prefixes differ from generated file`);
    const firstHeading=doc.querySelector("#documentation h2");
    if(firstHeading)assert(trigger.compareDocumentPosition(firstHeading)&dom.window.Node.DOCUMENT_POSITION_FOLLOWING,`${entry.path}: prefixes must precede the type/API headings`);
  } else assert(!trigger,`${entry.path}: data files must not advertise a snippet`);
  for (const id of [...oldIds,...original.matchAll(/<a\s+id="([^"]+)"/g)].map(x=>typeof x === "string"?x:x[1])) assert(doc.getElementById(id),`${entry.path}: lost anchor ${id}`);
  assert.deepEqual([...doc.querySelectorAll(".doc-body .code-wrap code")].map(c=>c.textContent),snippets,`${entry.path}: example changed`);
  assert.equal(doc.querySelectorAll(".toc").length,oldIds.length?1:0);
  assert.equal(doc.querySelectorAll('[role="tab"]').length,0);
  assert(![...doc.querySelectorAll("summary")].some(s=>s.textContent==="API一覧"));
  assert(!doc.querySelector(".katex-error"));
  dom.window.close();
}

const home = await page("index.html");
const d = home.document;
assert.equal(d.querySelectorAll(".library-row").length,index.length);
const reference=d.getElementById("snippet-reference");
assert(reference,"Missing snippet reference");
const referenceRows=[...reference.querySelectorAll(".snippet-reference-row")];
assert.equal(referenceRows.length,actualSnippets.size);
assert(d.querySelector('.index-guides a[href="#snippet-reference"]'),"Missing jump to snippet reference");
const lastCategory=[...d.querySelectorAll(".category-section")].at(-1);
assert(lastCategory.compareDocumentPosition(reference)&home.window.Node.DOCUMENT_POSITION_FOLLOWING,"Reference table must follow the library list");
for(const row of referenceRows){
  const actual=actualSnippets.get(row.dataset.path);
  assert(actual,row.dataset.path);
  assert.equal(row.querySelector("th a").textContent,actual.name);
  assert.deepEqual([...row.querySelectorAll("code")].map(code=>code.textContent),actual.prefixes,row.dataset.path);
  assert.equal(row.querySelector("th a").getAttribute("href"),config.siteBase+index.find(entry=>entry.path===row.dataset.path).url);
}
assert.equal(home.window.getComputedStyle(d.querySelector(".row-details")).display,"none");
d.querySelector("#catalog-details").click();
assert.equal(home.window.getComputedStyle(d.querySelector(".row-details")).display,"block");
const first = d.querySelector(".library-row");
first.querySelector("[data-pin]").click(); await tick();
d.querySelector("#pins-filter").click(); await tick();
assert.equal(d.querySelectorAll(".library-row:not([hidden])").length,1);
assert.equal(d.querySelector(".library-row:not([hidden])").dataset.path,first.dataset.path);
assert.equal(d.querySelectorAll(".snippet-reference-row:not([hidden])").length,1,"Snippet reference must follow the pin filter");
d.querySelector("#pins-filter").click(); await tick();
const search = d.querySelector("#catalog-search");
search.value = "zip_exact"; search.dispatchEvent(new home.window.Event("input")); await tick();
assert(d.querySelector('.library-row[data-path="DataStructureBasic/coordinate_compression.cpp"]:not([hidden])'),"Search must include API names hidden in compact mode");
search.value = "fps7"; search.dispatchEvent(new home.window.Event("input")); await tick();
assert(d.querySelector('.library-row[data-path="FPSandPolynomial/fps_mod1000000007.cpp"]:not([hidden])'),"Search must include snippet aliases");
assert(d.querySelector('.snippet-reference-row[data-path="FPSandPolynomial/fps_mod1000000007.cpp"]:not([hidden])'),"Reference table must follow prefix searches");
assert.equal(d.querySelectorAll(".snippet-reference-row:not([hidden])").length,1);
search.value = "no-such-library-fixture-937"; search.dispatchEvent(new home.window.Event("input")); await tick();
assert.equal(d.querySelectorAll(".library-row:not([hidden])").length,0);
assert.equal(d.querySelectorAll(".category-index a:not([hidden])").length,0);
assert.equal(d.querySelector("#catalog-empty").hidden,false);
assert.equal(d.querySelectorAll(".snippet-reference-row:not([hidden])").length,0);
assert.equal(d.querySelector("#snippet-reference-empty").hidden,false);
home.window.close();

const doc = await page("library/DataStructureBasic/rollback_union_find.html","#implementation");
assert.equal(doc.document.getElementById("implementation").open,true);
assert.equal(doc.document.getElementById("documentation").hidden,false);
doc.document.getElementById("implementation").open = false;
doc.document.querySelector('a[href="#implementation"]').click(); await tick();
assert.equal(doc.document.getElementById("implementation").open,true);
doc.document.querySelector("[data-copy-source]").click(); await tick();
assert.equal(doc.copied.at(-1),doc.document.getElementById("full-source").textContent);
doc.document.querySelector("[data-copy-url]").click(); await tick();
assert.equal(doc.copied.at(-1),index.find(e=>e.path==="DataStructureBasic/rollback_union_find.cpp") && JSON.parse(await fs.readFile(path.join(dist,"assets/source-manifest.json"),"utf8")).files.find(e=>e.path==="DataStructureBasic/rollback_union_find.cpp").url);
const supplement = doc.document.querySelector(".doc-supplement");
assert(supplement && !supplement.open);
const hashChanged = new Promise(resolve => doc.window.addEventListener("hashchange",resolve,{once:true}));
doc.window.location.hash = "#" + encodeURIComponent(supplement.querySelector("h2").id); await hashChanged;
assert.equal(supplement.open,true,"Old links into folded verification sections must reveal them");
assert.equal(doc.document.getElementById("rollbackunionfindmerge").textContent,"merge");
assert.equal(doc.document.getElementById("rollbackunionfindrollbackall").textContent,"rollback_all","Short labels must retain identifier underscores");
assert([...doc.document.querySelectorAll(".toc a")].some(a=>a.textContent==="rollback_all"));
doc.window.close();

const multi = new JSDOM(await fs.readFile(path.join(dist,"library/DataStructureSegment/segtree_wrappers.html"),"utf8"));
assert.equal(multi.window.document.getElementById("segminmaxset").textContent,"SegMinMax::set");
assert(multi.window.document.querySelector(".doc-body").textContent.includes("vector<ll>(n)"),"Template arguments must not disappear as HTML tags");
multi.window.close();
assert(renderProse("O(log(N+1))、0<=a,b<N").includes('class="katex"'));
assert(!renderProse("0<=t<=time()").includes('class="katex"'),"Do not typeset only part of an unsupported comparison");
assert(renderProse("0<=x<2^B").includes('2^B'),"Preserve the entire power bound in MathML's TeX annotation");
const katexCss = await fs.readFile(path.join(dist,"assets/katex/katex.min.css"),"utf8");
for (const match of katexCss.matchAll(/url\(([^)]+)\)/g)) await fs.access(path.join(dist,"assets/katex",match[1]));
console.log("Document checks passed: legacy anchors, unchanged examples, compact/detail views, API search, pin filters, hash disclosures, clipboard, math, and local fonts.");
