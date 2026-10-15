import katex from "katex";

const escapeHtml = text => String(text).replace(/[&<>"']/g, c => ({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"})[c]);
const mathCache = new Map();
export function renderMath(tex, displayMode = false) {
  const key = `${displayMode}:${tex}`;
  if (!mathCache.has(key)) mathCache.set(key, katex.renderToString(tex, {
    displayMode, throwOnError: true, strict: "error", trust: false, output: "htmlAndMathml"
  }));
  return mathCache.get(key);
}

// Only prose is passed here. Declarations, examples, inline code, and sources
// retain their original text, including ASCII operators and whitespace.
function toTex(expression) {
  return expression.replace(/[A-Za-z][A-Za-z0-9]*(?:_[A-Za-z0-9]+)?/g, name => {
    if (["log", "ln", "min", "max", "gcd", "exp"].includes(name)) return "\\" + name;
    if (name === "sigma") return "\\sigma";
    if (name.startsWith("log_")) return "\\log_{" + name.slice(4) + "}";
    if (name.length === 1) return name;
    return "\\mathit{" + name.replace(/_/g, "\\_") + "}";
  }).replace(/<=/g,"\\leq ").replace(/>=/g,"\\geq ").replace(/\*/g,"\\cdot ");
}

export function renderProse(text) {
  const candidates = [];
  // Balanced parentheses handle O(log(N+1)), O(min(N,M)), etc.
  const starts = /(?<![A-Za-z0-9_])O\(/g;
  for (const start of text.matchAll(starts)) {
    let end = start.index + 2, depth = 1;
    for (; end < text.length && depth; end++) {
      if (text[end] === "(") depth++;
      if (text[end] === ")") depth--;
    }
    const value = text.slice(start.index, end);
    if (!depth && /^O\([A-Za-z0-9_\s()+*/^,|.\-]+\)$/.test(value)) candidates.push({start:start.index,end,tex:toTex(value)});
  }
  // A deliberately small grammar: no inference about C++ calls or templates.
  const base = "(?:[A-Za-z][A-Za-z0-9_]*|[0-9]+)";
  const atom = base + "(?:\\^" + base + ")?(?:[+-]" + base + ")?";
  const operand = atom + "(?:," + atom + ")*";
  const comparison = new RegExp("(?<![A-Za-z0-9_])" + operand + "(?:\\s*(?:<=|>=|<|>)\\s*" + operand + ")+", "g");
  for (const match of text.matchAll(comparison)) {
    const start = match.index, end = start + match[0].length;
    // Leave expressions outside this grammar alone, rather than typesetting
    // only a misleading prefix (for example a bound ending in time()).
    if (/[A-Za-z0-9_^+*/(<>=-]/.test(text[end] || "")) continue;
    if (!candidates.some(c => start < c.end && end > c.start)) candidates.push({start,end,tex:toTex(match[0])});
  }
  let result = "", cursor = 0;
  for (const candidate of candidates.sort((a,b)=>a.start-b.start)) {
    if (candidate.start < cursor) continue;
    result += escapeHtml(text.slice(cursor, candidate.start));
    result += renderMath(candidate.tex);
    cursor = candidate.end;
  }
  return result + escapeHtml(text.slice(cursor));
}

export function prepareDocument(raw) {
  let text = raw.replace(/^# .*\r?\n/, "").replace(/^\[説明書の目次\].*\r?\n/gm, "");
  text = text.replace(/^型エイリアス・マクロとコードの置き方は \[共通設定\]\([^\n]+\) を参照してください。\s*$/gm, "");
  text = text.replace(/<details>\s*<summary>API一覧<\/summary>[\s\S]*?<\/details>\s*/g, "");
  // Keep the shared conditions visible, but give each condition its own line.
  text = text.replace(/(\*\*共通の前提\*\*\s*\n)([\s\S]*?)(?=\n## |\n<a |$)/, (_, label, body) => {
    const lines = body.replace(/^- (.+)$/gm, (line, value) => {
      if (!/[<>]=?/.test(value)) return line;
      return value.split(/、|。(?=.)/).map(part => part.trim()).filter(Boolean).map(part => "- " + part).join("\n");
    });
    return "**共通の制約**\n\n" + lines;
  });
  // Verification history stays accessible without competing with the API.
  const headings = [...text.matchAll(/^## (.+)$/gm)];
  const supplemental = /^(使用例の確認状況|確認結果|旧版からの移行)$/;
  const first = headings.findIndex(h => supplemental.test(h[1]));
  if (first >= 0 && headings.slice(first).every(h => supplemental.test(h[1]))) {
    const at = headings[first].index;
    text = text.slice(0, at) + '<details class="doc-supplement">\n<summary>検証記録・旧版からの移行</summary>\n\n' + text.slice(at).trimEnd() + "\n\n</details>\n";
  }
  return text.trim() + "\n";
}

export function headingLabels(text) {
  const titles = [...text.matchAll(/^#{2,3} (.+)$/gm)].map(m=>m[1]);
  const owners = new Set();
  let qualified = 0;
  for (const title of titles) {
    const match = title.match(/^([A-Za-z_]\w*(?:<[^>]+>)?)::(.+)$/);
    if (match) { owners.add(match[1]); qualified++; }
  }
  // Preserve qualifiers in wrapper collections with several different types.
  const owner = owners.size === 1 && qualified >= 2 ? [...owners][0] : null;
  return title => owner && title.startsWith(owner + "::") ? title.slice(owner.length + 2) : title;
}

export const mathExtensions = [{
  name: "inlineMath", level: "inline",
  start(src) { return src.indexOf("$"); },
  tokenizer(src) {
    const match = src.match(/^\$([^$\n]+?)\$(?!\$)/);
    if (match && match[1].trim() === match[1]) return {type:"inlineMath",raw:match[0],tex:match[1]};
  },
  renderer(token) { return renderMath(token.tex); }
}, {
  name: "displayMath", level: "block",
  start(src) { return src.indexOf("$$"); },
  tokenizer(src) {
    const match = src.match(/^\$\$\s*\n([\s\S]+?)\n\$\$(?:\n|$)/);
    if (match) return {type:"displayMath",raw:match[0],tex:match[1]};
  },
  renderer(token) { return renderMath(token.tex, true) + "\n"; }
}];
