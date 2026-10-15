(() => {
  "use strict";
  const $ = (s, root = document) => root.querySelector(s);
  const $$ = (s, root = document) => Array.from(root.querySelectorAll(s));
  const base = document.body.dataset.base;
  const storage = {
    read(key, fallback) { try { return JSON.parse(localStorage.getItem(key)) ?? fallback; } catch { return fallback; } },
    write(key, value) { try { localStorage.setItem(key, JSON.stringify(value)); } catch {} }
  };
  let toastTimer;
  function toast(message) {
    $("#toast").textContent = message; $("#toast").classList.add("visible");
    clearTimeout(toastTimer); toastTimer = setTimeout(() => $("#toast").classList.remove("visible"), 2400);
  }
  async function copy(text) {
    try {
      if (navigator.clipboard && window.isSecureContext) await navigator.clipboard.writeText(text);
      else {
        const area = document.createElement("textarea"); area.value = text;
        area.style.position = "fixed"; area.style.opacity = "0"; document.body.append(area);
        area.select(); const ok = document.execCommand("copy"); area.remove();
        if (!ok) throw new Error("Copy failed");
      }
      toast("コピーしました");
    } catch { toast("コピーできませんでした。文字列を選択してコピーしてください。"); }
  }
  document.documentElement.dataset.theme = "light";
  function toggleMenu(open) {
    document.body.classList.toggle("nav-open", open);
    $("#menu-toggle").setAttribute("aria-expanded", String(open)); $("#nav-shade").hidden = !open;
  }
  $("#menu-toggle").addEventListener("click", () => toggleMenu(!document.body.classList.contains("nav-open")));
  $("#nav-shade").addEventListener("click", () => toggleMenu(false));
  $("#menu-close").addEventListener("click", () => toggleMenu(false));
  $$("#sidebar a").forEach(a => a.addEventListener("click", () => toggleMenu(false)));
  $$("[data-copy-source]").forEach(b => b.addEventListener("click", () => copy($("#full-source").textContent)));
  $$("[data-copy-url]").forEach(b => b.addEventListener("click", () => copy(b.dataset.copyUrl)));
  $$(".code-wrap").forEach(wrapper => {
    const button = document.createElement("button"); button.type = "button"; button.className = "copy-snippet";
    button.textContent = "コピー"; button.setAttribute("aria-label", "このコード例をコピー");
    button.addEventListener("click", () => copy($("code", wrapper).textContent)); wrapper.append(button);
  });
  const pins = new Set(storage.read("library-pins", []));
  function refreshPins() {
    $$("[data-pin]").forEach(b => {
      const pinned = pins.has(b.dataset.pin); b.setAttribute("aria-pressed", String(pinned));
      b.textContent = pinned ? "★" : "☆";
      b.setAttribute("aria-label", pinned ? "ピン留めを解除" : "ピン留めする"); b.title = b.getAttribute("aria-label");
    });
  }
  $$("[data-pin]").forEach(b => b.addEventListener("click", () => {
    const key = b.dataset.pin; pins.has(key) ? pins.delete(key) : pins.add(key);
    storage.write("library-pins", Array.from(pins)); refreshPins(); applyFilter();
  }));
  refreshPins();
  let pinsOnly = false;
  let searchIndex;
  let filterRequest=0;
  let searchRequest=0;
  function normalize(value) { return String(value).normalize("NFKC").toLowerCase().replace(/\s+/g, " ").trim(); }
  function matches(item, query) {
    const haystack = normalize(item.title + " " + item.path + " " + item.description + " " + item.category + " " + item.text);
    return normalize(query).split(" ").filter(Boolean).every(word => haystack.includes(word));
  }
  async function loadIndex() {
    if (!searchIndex) {
      const response = await fetch(base + "assets/search.json");
      if (!response.ok) throw new Error("Search index could not be loaded");
      searchIndex = await response.json();
    }
    return searchIndex;
  }
  const inlineSearch = $("#catalog-search");
  const detailsToggle = $("#catalog-details");
  if (detailsToggle) {
    function showDetails(show) {
      $(".overview").classList.toggle("show-details", show);
      detailsToggle.setAttribute("aria-pressed", String(show));
      detailsToggle.textContent = show ? "説明を隠す" : "説明を表示";
      storage.write("catalog-details", show);
    }
    showDetails(storage.read("catalog-details", false) === true);
    detailsToggle.addEventListener("click", () => showDetails(detailsToggle.getAttribute("aria-pressed") !== "true"));
  }
  async function applyFilter() {
    if (!inlineSearch) return;
    const request=++filterRequest;
    const query = inlineSearch.value;
    let index;
    try { index = await loadIndex(); }
    catch { index = $$(".library-row").map(row => ({ path: row.dataset.path, title: row.dataset.title, category: row.dataset.category, description: row.dataset.description, text: "" })); }
    if(request!==filterRequest)return;
    const paths = new Set(index.filter(item => matches(item, query) && (!pinsOnly || pins.has(item.path))).map(item => item.path));
    $$(".library-row").forEach(row => { row.hidden = !paths.has(row.dataset.path); });
    $$(".category-section").forEach(section => {
      section.hidden = !$(".library-row:not([hidden])", section);
      const jump = $('.category-index a[href="#' + section.id + '"]');
      if (jump) jump.hidden = section.hidden;
    });
    $("#catalog-count").textContent = paths.size + " 件";
    $("#catalog-empty").hidden = paths.size > 0;
  }
  if (inlineSearch) {
    inlineSearch.addEventListener("input", applyFilter);
    $("#pins-filter").addEventListener("click", () => {
      pinsOnly = !pinsOnly; $("#pins-filter").setAttribute("aria-pressed", String(pinsOnly)); applyFilter();
    });
  }
  function revealHash(hash) {
    let id; try { id = decodeURIComponent(hash.slice(1)); } catch { return; }
    const target = document.getElementById(id);
    if (!target) return;
    let container = target.closest("details"), opened = false;
    while (container) {
      if (!container.open) { container.open = true; opened = true; }
      container = container.parentElement?.closest("details");
    }
    return opened ? target : null;
  }
  function followHash() {
    const target = revealHash(location.hash);
    if (target) requestAnimationFrame(() => target.scrollIntoView({block:"start"}));
  }
  followHash(); window.addEventListener("hashchange", followHash);
  document.addEventListener("click", event => {
    const link = event.target.closest("a[href]");
    if (link && link.origin === location.origin && link.pathname === location.pathname && link.hash) revealHash(link.hash);
  });
  const searchDialog = $("#search-dialog");
  let selectedResult = -1;
  function chooseResult(index) {
    const links = $$(".search-result", $("#search-results"));
    if (!links.length) return;
    selectedResult = Math.max(0, Math.min(index, links.length - 1));
    links.forEach((link, i) => link.classList.toggle("selected", i === selectedResult));
    links[selectedResult].scrollIntoView({block: "nearest"});
  }
  async function showSearchResults() {
    const request=++searchRequest;
    const query = $("#global-search").value; const results = $("#search-results"); results.replaceChildren(); selectedResult = -1;
    try {
      const index = await loadIndex();
      if(request!==searchRequest)return;
      const items = index.filter(item => matches(item, query)).sort((a, b) => Number(normalize(b.title).includes(normalize(query))) - Number(normalize(a.title).includes(normalize(query)))).slice(0, 40);
      $("#search-status").textContent = items.length ? items.length + " 件を表示" : "一致するライブラリがありません。別の名前や関数名を試してください。";
      for (const item of items) {
        const a = document.createElement("a"); a.className = "search-result"; a.href = base + item.url;
        const title = document.createElement("strong"); title.textContent = item.title;
        const category = document.createElement("span"); category.className = "search-category"; category.textContent = item.category; title.append(category);
        const path = document.createElement("small"); path.textContent = item.path;
        const description = document.createElement("p"); description.textContent = item.description;
        a.append(title, path, description); results.append(a);
      }
    } catch { $("#search-status").textContent = "検索データを読み込めませんでした。ページを再読み込みしてください。"; }
  }
  $$("[data-open-search]").forEach(b => b.addEventListener("click", async () => {
    if (!searchDialog.open) searchDialog.showModal(); $("#global-search").focus(); await showSearchResults();
  }));
  $("#close-search").addEventListener("click", () => searchDialog.close());
  searchDialog.addEventListener("click", event => {
    const r = searchDialog.getBoundingClientRect();
    if (event.clientX < r.left || event.clientX > r.right || event.clientY < r.top || event.clientY > r.bottom) searchDialog.close();
  });
  $("#global-search").addEventListener("input", showSearchResults);
  $("#global-search").addEventListener("keydown", event => {
    if (event.key === "ArrowDown" || event.key === "ArrowUp") { event.preventDefault(); chooseResult(selectedResult + (event.key === "ArrowDown" ? 1 : -1)); }
    if (event.key === "Enter") { const link = $$(".search-result")[Math.max(0, selectedResult)]; if (link) location.href = link.href; }
  });
  document.addEventListener("keydown", event => {
    const typing = /INPUT|TEXTAREA|SELECT/.test(document.activeElement.tagName) || document.activeElement.isContentEditable;
    if ((event.key.toLowerCase() === "k" && (event.metaKey || event.ctrlKey)) || (event.key === "/" && !typing)) {
      event.preventDefault();
      if (inlineSearch) inlineSearch.focus();
      else $("[data-open-search]").click();
    }
    if (event.key === "Escape") {
      toggleMenu(false);
      if (inlineSearch && document.activeElement === inlineSearch) { inlineSearch.value = ""; applyFilter(); inlineSearch.blur(); }
    }
  });
  if ("IntersectionObserver" in window) {
    const headingLinks = new Map($$(".toc a").map(a => [decodeURIComponent(a.hash.slice(1)), a]));
    const observer = new IntersectionObserver(entries => {
      for (const entry of entries) if (entry.isIntersecting) {
        headingLinks.forEach(a => a.classList.remove("is-current"));
        headingLinks.get(entry.target.id)?.classList.add("is-current");
      }
    }, {rootMargin: "-90px 0px -65% 0px"});
    $$(".doc-body h2[id], .doc-body h3[id]").forEach(heading => observer.observe(heading));
  }
})();
