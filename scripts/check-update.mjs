import fs from "node:fs/promises";
const config=JSON.parse(await fs.readFile("site.config.json","utf8"));
let changed=true;
if(process.env.GITHUB_EVENT_NAME==="schedule"){
  try{
    const url=config.siteOrigin+config.siteBase+"assets/source-manifest.json?t="+Date.now();
    const response=await fetch(url,{headers:{"Cache-Control":"no-cache"},signal:AbortSignal.timeout(15000)});
    if(response.ok){
      const manifest=await response.json();
      changed=manifest.commit!==process.env.SOURCE_SHA;
    }
  }catch(error){console.log("Could not verify deployed revision; rebuilding:",error.message);}
}
if(process.env.GITHUB_OUTPUT)await fs.appendFile(process.env.GITHUB_OUTPUT,"changed="+changed+"\n");
console.log(changed?"Build required":"Deployed source is already up to date");
