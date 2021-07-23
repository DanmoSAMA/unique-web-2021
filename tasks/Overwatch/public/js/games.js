const gamesBtn = document.getElementById("games-btn");
const dropDown = document.getElementById("games-dropDown");
const mask = document.getElementById("mask");
const icon = document.getElementById("games-downArrow");

dropDown.style.width = document.body.clientWidth + "px";
window.addEventListener('resize', resize);
gamesBtn.addEventListener('click', click);
window.addEventListener("resize", resize);

function click(event) {
  dropDown.className = "selected";
  mask.className = "show";
  icon.innerHTML = "&#xe602;";
  event.stopPropagation();
  document.addEventListener("click", anotherClick);
}

function anotherClick(event) {
  const tDom = event.target;
  if (tDom !== dropDown && !dropDown.contains(tDom)) {
    dropDown.className = "";
    mask.className = "";
    icon.innerHTML = "&#xe60b;";
  }
}

function resize() {
  dropDown.className = "";
  mask.className = "";
  icon.innerHTML = "&#xe60b;";
}