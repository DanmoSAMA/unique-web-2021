const nav = document.getElementById("topbar-mobileNav");
const games = document.getElementById("topbar-mobileNav-games");
const mask = document.getElementById("mask");
const gamesBtn = document.getElementById("topbar-mobileNav-games-btn");
const closeBtn = document.getElementById("close-btn");
const homeBtn = document.getElementById("home");

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
  window.removeEventListener("mousewheel", banScroll);
}

function load(event) {
  nav.className = "loaded";
  mask.className = "show";
  event.stopPropagation();
  document.addEventListener("click", anotherClick);

  window.addEventListener("mousewheel", banScroll, {
    //passive表示listener永远不会调用preventDefault()
    passive: false
  });
}

function anotherClick(event) {
  const tDom = event.target;
  if (tDom !== nav && !nav.contains(tDom)) {
    close();
  }
}

function banScroll(event) {
  event.preventDefault();
}
