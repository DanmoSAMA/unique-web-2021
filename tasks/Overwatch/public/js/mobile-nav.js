const bodyEl = document.body;
const nav = document.getElementById("topbar-mobileNav");
const games = document.getElementById("topbar-mobileNav-games");
const mask = document.getElementById("mask");
const gamesBtn = document.getElementById("topbar-mobileNav-games-btn");
const closeBtn = document.getElementById("close-btn");
const homeBtn = document.getElementById("home");

let top = 0;

gamesBtn.addEventListener("click", toggleSlide);
closeBtn.addEventListener("click", close);
homeBtn.addEventListener("click", load);
window.addEventListener("resize", close);

function toggleSlide() {
  if (games.className === "topbar-mobileNav-item") {
    games.className = "topbar-mobileNav-item selected";
  } else {
    games.className = "topbar-mobileNav-item";
  }
}

function close() {
  nav.className = "";
  mask.className = "";
  games.className = "topbar-mobileNav-item";
  document.removeEventListener("mousewheel", banScroll);

  // 恢复滚动，保持原先的位置
  bodyEl.style.position = "";
  bodyEl.style.top = "";
  window.scrollTo(0, top);
}

function load(event) {
  nav.className = "loaded";
  mask.className = "show";
  event.stopPropagation();
  document.addEventListener("click", anotherClick);

  // 禁止蒙层底部页面跟随滚动
  top = window.scrollY;
  bodyEl.style.position = "fixed";
  bodyEl.style.top = -top + "px";
}

function anotherClick(event) {
  const tDom = event.target;
  if (tDom !== nav && !nav.contains(tDom)) {
    close();
    document.removeEventListener("click", anotherClick);
  }
}

function banScroll(event) {
  event.preventDefault();
}
