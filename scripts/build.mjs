import fs from "node:fs/promises";
import path from "node:path";
import { execFileSync } from "node:child_process";
import { createHash } from "node:crypto";
import { Marked } from "marked";

const ROOT = process.cwd();
const config = JSON.parse(await fs.readFile("site.config.json", "utf8"));
const catalog = JSON.parse(await fs.readFile("catalog.json", "utf8"));
const documentationRevisions = JSON.parse(await fs.readFile("documentation-revisions.json", "utf8"));
const sourceRoot = path.resolve(process.env.SOURCE_DIR || "source");
const out = path.resolve(process.env.OUTPUT_DIR || "dist");
if(out===ROOT || out===sourceRoot || out===path.parse(out).root || sourceRoot.startsWith(out+path.sep))throw new Error("Unsafe output directory");
const base = config.siteBase;
if (!/^\/[a-zA-Z0-9/_-]+\/$/.test(base)) throw new Error("Invalid siteBase");
let revision = process.env.SOURCE_SHA || "";
if (!revision) {
  try { revision = execFileSync("git", ["-C", sourceRoot, "rev-parse", "HEAD"], {encoding: "utf8"}).trim(); }
  catch { throw new Error("Set SOURCE_SHA when the source directory is not a Git checkout."); }
}
if (!/^[a-f0-9]{40}$/.test(revision)) throw new Error("SOURCE_SHA must be a full commit SHA");
const sourceUrl = "https://github.com/" + config.sourceRepository + "/blob/" + revision + "/";
const buildTime = process.env.SOURCE_DATE || new Date().toISOString();
const displayDate = new Date(buildTime).toLocaleString("ja-JP", {timeZone:"Asia/Tokyo",year:"numeric",month:"2-digit",day:"2-digit",hour:"2-digit",minute:"2-digit",hour12:false});
const esc = value => String(value).replace(/[&<>"']/g, c => ({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"})[c]);
const plain = value => String(value).replace(/<[^>]*>/g, " ").replace(/!?\[([^\]]+)\]\([^)]*\)/g, "$1").replace(/[\x60*#_]/g, "").replace(/\s+/g, " ").trim();
const hrefPath = value => value.split("/").map(encodeURIComponent).join("/");
const docPathFor = source => "usage/" + source.replace(/\.(cpp|txt)$/, ".md");
const pagePathFor = source => "library/" + source.replace(/\.(cpp|txt)$/, ".html");
async function write(relative, value) {
  const target = path.join(out, relative);
  await fs.mkdir(path.dirname(target), {recursive:true}); await fs.writeFile(target, value);
}
async function walk(dir, prefix="") {
  const found=[];
  for (const item of await fs.readdir(dir,{withFileTypes:true})) {
    if (item.name.startsWith(".")) continue;
    const relative=path.posix.join(prefix,item.name);
    if (item.isSymbolicLink()) throw new Error("Symlinks are not allowed: " + relative);
    if (item.isDirectory()) found.push(...await walk(path.join(dir,item.name),relative));
    else found.push(relative);
  }
  return found.sort();
}
const codePaths = (await walk(sourceRoot)).filter(p=>p.endsWith(".cpp"));
if (!codePaths.length) throw new Error("Source repository contains no C++ sources");
const codeSet = new Set(codePaths);
const dataItems = catalog.filter(x=>x.path.endsWith(".txt"));
const entries = catalog.filter(x=>codeSet.has(x.path) || x.path.endsWith(".txt"));
for (const source of codePaths) if (!entries.some(x=>x.path===source)) {
  const folder=source.includes("/") ? source.split("/")[0] : "追加ライブラリ";
  const peer=catalog.find(x=>x.categoryId===folder);
  entries.push({path:source,title:path.basename(source,".cpp"),description:"実装と公開ソースを参照できます。",category:peer?.category || folder,categoryId:peer?.categoryId || folder});
}
const categories = [];
for (const entry of entries) if(!categories.some(x=>x.id===entry.categoryId)) categories.push({id:entry.categoryId,title:entry.category});
const codeBuffers=new Map();
for(const source of codePaths) codeBuffers.set(source,await fs.readFile(path.join(sourceRoot,source)));
for(const item of dataItems) codeBuffers.set(item.path,await fs.readFile(path.join(ROOT,"data",path.basename(item.path))));
await fs.rm(out,{recursive:true,force:true}); await fs.mkdir(out,{recursive:true});
for (const name of ["style.css","app.js","favicon.svg"]) await write("assets/"+name, await fs.readFile("web/"+name));
const shell = await fs.readFile("web/shell.html","utf8");
const docTargets = new Map([
  ["docs/usage/index.md","index.html"],
  ["README.md","index.html"],
  ["docs/usage/common.md","guide/common.html"],
  ["docs/usage/validation.md","guide/validation.html"],
  ["docs/review-notes.md","guide/review-notes.html"],
  ["docs/publishing.md","guide/publishing.html"],
  ["docs/file-map.md","guide/file-map.html"],
  ["docs/file-manifest.json","assets/file-manifest.json"]
]);
for(const item of entries) docTargets.set("docs/"+docPathFor(item.path),pagePathFor(item.path));
function rewriteLink(href, currentDoc) {
  if (!href) return "#";
  const historical=href.match(/^https:\/\/github\.com\/yuki-4869\/Algorithm\/blob\/[a-f0-9]{40}\/(.+)$/);
  if(historical){
    return href.replace("/yuki-4869/Algorithm/blob/","/"+config.archiveRepository+"/blob/");
  }
  const branchSource=href.match(/^https:\/\/github\.com\/yuki-4869\/Algorithm\/blob\/main\/(.+)$/);
  if(branchSource) return sourceUrl+hrefPath(branchSource[1]);
  if (/^(https?:|mailto:|#)/i.test(href)) return href;
  if (/^[a-z][a-z0-9+.-]*:/i.test(href)) return "#";
  const [raw,fragment] = href.split("#");
  let decoded; try {decoded=decodeURIComponent(raw);} catch {decoded=raw;}
  const resolved=path.posix.normalize(path.posix.join(path.posix.dirname(currentDoc),decoded));
  if(resolved.startsWith("tests/"))return "https://github.com/"+config.archiveRepository+"/blob/"+config.archiveCommit+"/"+hrefPath(resolved);
  let target=docTargets.get(resolved);
  if(!target && codeSet.has(resolved)) target=pagePathFor(resolved);
  if(!target && resolved.endsWith(".txt")) target="assets/data/"+path.basename(resolved);
  if(!target && resolved.endsWith("/")) target="index.html";
  if(!target) {
    if(catalog.some(item=>item.path===resolved))return "https://github.com/"+config.archiveRepository+"/blob/"+config.archiveCommit+"/"+hrefPath(resolved);
    if(resolved.endsWith(".cpp")) return "https://github.com/"+config.sourceRepository+"/blob/"+revision+"/"+hrefPath(resolved);
    if(resolved.endsWith(".md")) return base;
    return href;
  }
  return base+target+(fragment ? "#"+fragment : (codeSet.has(resolved) ? "#implementation" : ""));
}
function highlightCpp(code) {
  const tokens = /(\/\/[^\n]*|\/\*[\s\S]*?\*\/|"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'|^[ \t]*#[^\n]*|\b(?:alignas|alignof|auto|bool|break|case|catch|char|class|concept|const|consteval|constexpr|constinit|continue|decltype|default|delete|do|double|else|enum|explicit|export|extern|false|float|for|friend|if|inline|int|long|namespace|new|noexcept|nullptr|operator|private|protected|public|register|requires|return|short|signed|sizeof|static|static_assert|struct|switch|template|this|thread_local|throw|true|try|typedef|typename|union|unsigned|using|virtual|void|volatile|while)\b|\b(?:0x[0-9a-fA-F]+|\d+(?:\.\d+)?(?:[eE][+-]?\d+)?)[uUlLfF]*\b)/gm;
  let result="",last=0;
  for(const match of code.matchAll(tokens)){
    result+=esc(code.slice(last,match.index));
    const text=match[0];
    const kind=text.startsWith("//")||text.startsWith("/*") ? "comment" : /^["']/.test(text) ? "string" : /^\s*#/.test(text) ? "preprocessor" : /^\d/.test(text) ? "number" : "keyword";
    result+='<span class="token-'+kind+'">'+esc(text)+"</span>";last=match.index+text.length;
  }
  return result+esc(code.slice(last));
}
function renderMarkdown(text,currentDoc) {
  const toc=[];
  const used = new Set(Array.from(text.matchAll(/<a\s+id="([^"]+)"/g),m=>m[1]));
  const renderer={
    heading(token) {
      const title=plain(token.text);
      const raw=title.normalize("NFKC").toLowerCase().replace(/[^\p{L}\p{N}\s_-]/gu,"").trim().replace(/\s+/g,"-")||"section";
      let id=raw,n=1; while(used.has(id))id=raw+"-"+n++;used.add(id);
      if(token.depth===2||token.depth===3)toc.push({title,id,depth:token.depth});
      return "<h"+token.depth+' id="'+esc(id)+'">'+this.parser.parseInline(token.tokens)+"</h"+token.depth+">\n";
    },
    link(token) {
      const url=rewriteLink(token.href,currentDoc);
      const external=/^https?:/.test(url);
      return '<a href="'+esc(url)+'"'+(external?' target="_blank" rel="noopener noreferrer"':"")+">"+this.parser.parseInline(token.tokens)+"</a>";
    },
    code(token) {
      const lang=(token.lang||"").split(/\s/)[0];
      const body=/^(cpp|c\+\+|c|cc)$/.test(lang) ? highlightCpp(token.text) : esc(token.text);
      return '<div class="code-wrap"><pre><code class="language-'+esc(lang||"text")+'">'+body+"</code></pre></div>\n";
    }
  };
  const marked=new Marked({gfm:true,renderer});
  const html=marked.parse(text).replace(/<table>/g,'<div class="table-wrap"><table>').replace(/<\/table>/g,"</table></div>");
  return {html,toc};
}
function navigation(current) {
  return categories.map((category,i)=>{
    const members=entries.filter(x=>x.categoryId===category.id);
    const active=current?.categoryId===category.id;
    const anchor=base+"#category-"+(i+1);
    let body='<div class="nav-category"><a href="'+anchor+'"'+(active?' class="active"':"")+"><span>"+esc(category.title)+'</span><span class="nav-count">'+members.length+"</span></a>";
    if(active)body+='<div class="nav-children">'+members.map(item=>'<a href="'+base+pagePathFor(item.path)+'"'+(current.path===item.path?' class="active" aria-current="page"':"")+">"+esc(item.title)+"</a>").join("")+"</div>";
    return body+"</div>";
  }).join("");
}
function footer() {
  return '<footer class="site-footer"><span>yuki4869 Algorithm Library <span class="footer-separator">/</span> C++ Reference</span><span>ソース同期 '+esc(displayDate)+' JST <span class="footer-separator">·</span><a href="https://github.com/'+config.sourceRepository+'/commit/'+revision+'">'+revision.slice(0,7)+"</a></span></footer>";
}
function layout({title,description="",id="",content,current=null,home=false,mainClass=""}) {
  const slots={TITLE:esc(title),DESCRIPTION:esc(description),SOURCE_SHA:revision,SHORT_SHA:revision.slice(0,7),BASE:base,PAGE_ID:esc(id),COUNT:codePaths.length,HOME_ACTIVE:home?"active":"",NAV:navigation(current),MAIN_CLASS:mainClass,CONTENT:content};
  return shell.replace(/@@([A-Z_]+)@@/g,(_,key)=>slots[key]??"");
}
const searchIndex=[];
const manifest=[];
const documentRecords=[];
for(const entry of entries) {
  const isData=entry.path.endsWith(".txt");
  let raw;
  try {raw=await fs.readFile(path.join(ROOT,"content",docPathFor(entry.path)),"utf8");}
  catch {raw="# "+entry.title+"\n\nこのライブラリの実装を参照できます。\n\n## 使い方\n\n実装タブでコードを確認してください。API・制約・計算量の説明は追記予定です。\n";}
  raw=raw.replace(/^# .*\r?\n/,"").replace(/^\[説明書の目次\].*\r?\n/gm,"");
  const rendered=renderMarkdown(raw,"docs/"+docPathFor(entry.path));
  const source=codeBuffers.get(entry.path);
  const text=source.toString("utf8");
  const blob=createHash("sha1").update("blob "+source.length+"\0").update(source).digest("hex");
  const stale=!isData && documentationRevisions[entry.path]!==blob;
  const codeUrl=isData ? config.siteOrigin+base+"assets/data/"+path.basename(entry.path) : sourceUrl+hrefPath(entry.path);
  const sourceName=isData ? "データ" : "実装";
  const content='<div class="breadcrumb"><a href="'+base+'">ライブラリ一覧</a><span aria-hidden="true">/</span><a href="'+base+'#category-'+(categories.findIndex(x=>x.id===entry.categoryId)+1)+'">'+esc(entry.category)+"</a></div>"+
    '<header class="doc-heading"><div class="page-label">'+esc(entry.category)+'</div><div class="page-heading"><div><h1>'+esc(entry.title)+'</h1><div class="doc-path">'+esc(entry.path)+'</div><p class="page-lead">'+esc(entry.description)+'</p></div><span class="version-chip">source <span class="mono">'+revision.slice(0,7)+'</span></span></div>'+
    '<div class="doc-actions"><button class="action-button primary" data-copy-source>コードをコピー</button><button class="action-button" data-copy-url="'+esc(codeUrl)+'">参照URLをコピー</button><a class="action-button" href="'+esc(codeUrl)+'" target="_blank" rel="noopener noreferrer">'+(isData?"データを開く":"GitHubで開く ↗")+'</a><button class="action-button" data-pin="'+esc(entry.path)+'" aria-pressed="false" aria-label="ピン留めする">☆</button></div></header>'+
    '<div class="doc-layout"><div class="doc-main"><div class="tabs" role="tablist" aria-label="表示内容"><button class="tab" id="tab-documentation" role="tab" data-tab="documentation" aria-controls="documentation" aria-selected="true">使い方<span class="tab-count">'+rendered.toc.length+'</span></button><button class="tab" id="tab-implementation" role="tab" data-tab="implementation" aria-controls="implementation" aria-selected="false" tabindex="-1">'+sourceName+'</button></div>'+
    '<section id="documentation" role="tabpanel" aria-labelledby="tab-documentation" class="doc-body">'+
    (stale?'<blockquote><p>公開コードが更新されています。この説明は更新前の内容を含む可能性があります。実装タブで現在のコードを確認してください。</p></blockquote>':"")+rendered.html+'</section>'+
    '<section id="implementation" role="tabpanel" aria-labelledby="tab-implementation" hidden><div class="source-panel-bar"><span>'+esc(entry.path)+' · '+text.split("\n").length+' lines</span><a class="action-button" href="'+base+'sources/'+hrefPath(entry.path)+'" download>ダウンロード ↓</a></div><pre class="source-code"><code id="full-source">'+(isData?esc(text):highlightCpp(text))+'</code></pre><p class="source-note">'+(isData?"素数表はテキストデータです。先頭の説明行を除いて利用してください。":"公開リポジトリの上記コミットと同じコードです。必要な共通定義は「共通設定・添字」を確認してください。")+'</p></section>'+
    '<nav class="doc-pagination" aria-label="前後のライブラリ">'+
    (entries.indexOf(entry)>0?'<a href="'+base+pagePathFor(entries[entries.indexOf(entry)-1].path)+'"><small>← PREVIOUS</small>'+esc(entries[entries.indexOf(entry)-1].title)+"</a>":"<span></span>")+
    (entries.indexOf(entry)<entries.length-1?'<a href="'+base+pagePathFor(entries[entries.indexOf(entry)+1].path)+'"><small>NEXT →</small>'+esc(entries[entries.indexOf(entry)+1].title)+"</a>":"")+'</nav>'+footer()+'</div>'+
    '<aside class="toc" aria-label="このページの目次"><p class="eyebrow">ON THIS PAGE</p>'+rendered.toc.map(h=>'<a href="#'+encodeURIComponent(h.id)+'" class="level-'+h.depth+'">'+esc(h.title)+"</a>").join("")+"</aside></div>";
  await write(pagePathFor(entry.path),layout({title:entry.title,description:entry.description,id:entry.path,content,current:entry}));
  await write("sources/"+entry.path,source);
  if(isData)await write("assets/data/"+path.basename(entry.path),source);
  searchIndex.push({...entry,url:pagePathFor(entry.path),text:plain(raw).slice(0,42000)});
  if(!isData)manifest.push({path:entry.path,sha256:createHash("sha256").update(source).digest("hex"),blob:createHash("sha1").update("blob "+source.length+"\0").update(source).digest("hex"),bytes:source.length,url:codeUrl});
  documentRecords.push({source:entry.path,output:pagePathFor(entry.path),headings:rendered.toc.length});
}
const header='<div class="breadcrumb"><span>yuki4869</span><span aria-hidden="true">/</span><span>Reference</span></div><header class="page-heading"><div><div class="page-label">ALGORITHM REFERENCE</div><h1>ライブラリ一覧</h1><p class="page-lead">実装・API・制約・計算量を、すぐ参照できるように。</p></div><span class="version-chip">source <span class="mono">'+revision.slice(0,7)+'</span></span></header>'+
  '<div class="overview-stats"><span><strong>'+codePaths.length+'</strong>C++コード</span><span><strong>'+categories.length+'</strong>分類</span><span><strong>C++23</strong></span><span class="last-update">ソース同期 '+esc(displayDate)+' JST</span></div>'+
  '<div class="overview-tools"><label class="inline-search"><span aria-hidden="true">⌕</span><input type="search" id="catalog-search" placeholder="名前・関数・用途で絞り込み…" aria-label="ライブラリを絞り込む" autocomplete="off"><kbd>/</kbd></label><button class="filter-button" id="pins-filter" aria-pressed="false">☆ ピン留め</button></div>'+
  '<div class="result-info"><span id="catalog-count" aria-live="polite">'+entries.length+' 件</span><span>APIから探すなら <button data-open-search class="text-button">全体検索 ↗</button></span></div>';
let sections="";
for(let i=0;i<categories.length;i++){
  const category=categories[i],items=entries.filter(x=>x.categoryId===category.id);
  sections+='<section class="category-section" id="category-'+(i+1)+'"><div class="category-title"><span class="category-number">'+String(i+1).padStart(2,"0")+'</span><h2>'+esc(category.title)+'</h2><span class="category-count">'+items.length+' items</span></div><div class="catalog-list">'+
    items.map(item=>'<div class="library-row" data-path="'+esc(item.path)+'" data-title="'+esc(item.title)+'" data-description="'+esc(item.description)+'" data-category="'+esc(item.category)+'"><a class="library-link" href="'+base+pagePathFor(item.path)+'">'+esc(item.title)+(item.path.endsWith(".txt")?'<span class="data-tag">DATA</span>':"")+'<small>'+esc(item.path)+'</small></a><span class="row-description">'+esc(item.description)+'</span><button class="pin-button" data-pin="'+esc(item.path)+'" aria-pressed="false" aria-label="ピン留めする">☆</button></div>').join("")+'</div></section>';
}
await write("index.html",layout({title:"ライブラリ一覧",description:"競技プログラミング用C++ライブラリのAPI・制約・計算量・使用例",id:"index",home:true,mainClass:"overview",content:header+sections+'<p id="catalog-empty" class="empty-state" hidden>該当するライブラリがありません。検索語やピン留めの条件を変えてください。</p>'+footer()}));
const guides=[
  ["common","usage/common.md","共通設定・添字"],
  ["validation","usage/validation.md","検証記録"],
  ["review-notes","review-notes.md","確認事項・既知の制約"],
  ["publishing","publishing.md","更新・参照URLについて"],
  ["file-map","file-map.md","元ファイル名との対応"]
];
for(const [slug,file,title] of guides){
  const raw=(await fs.readFile("content/"+file,"utf8")).replace(/^# .*\r?\n/,"");
  const rendered=renderMarkdown(raw,"docs/"+file);
  const content='<div class="breadcrumb"><a href="'+base+'">ライブラリ一覧</a><span aria-hidden="true">/</span><span>Guide</span></div><header class="page-heading"><div><div class="page-label">GUIDE</div><h1>'+esc(title)+'</h1></div></header><div class="doc-layout"><div class="doc-main"><article class="doc-body">'+rendered.html+'</article>'+footer()+'</div><aside class="toc" aria-label="このページの目次"><p class="eyebrow">ON THIS PAGE</p>'+rendered.toc.map(h=>'<a href="#'+encodeURIComponent(h.id)+'" class="level-'+h.depth+'">'+esc(h.title)+"</a>").join("")+"</aside></div>";
  await write("guide/"+slug+".html",layout({title,id:"guide/"+slug,content}));
}
await write("assets/search.json",JSON.stringify(searchIndex));
await write("assets/source-manifest.json",JSON.stringify({repository:config.sourceRepository,commit:revision,builtAt:buildTime,files:manifest},null,2)+"\n");
await write("assets/build-manifest.json",JSON.stringify({sourceCommit:revision,codeCount:codePaths.length,pageCount:documentRecords.length,categories:categories.length,documents:documentRecords},null,2)+"\n");
await write("assets/file-manifest.json",await fs.readFile("content/file-manifest.json"));
await write(".nojekyll","");
await write("404.html",layout({title:"ページが見つかりません",content:'<div class="page-label">404</div><div class="page-heading"><h1>ページが見つかりません</h1></div><p class="page-lead">URLが変わったか、ライブラリが削除された可能性があります。</p><p><a class="action-button" href="'+base+'">ライブラリ一覧へ戻る</a></p>'}));
console.log(JSON.stringify({sourceCommit:revision,codeCount:codePaths.length,libraryPages:entries.length,categories:categories.length,output:out}));
