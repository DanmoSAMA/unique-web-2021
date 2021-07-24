const dropDown = document.getElementById("account-dropDown");
const accountBtn = document.getElementById("account-btn");
const mask = document.getElementById("mask");
const icon = document.getElementById("account-downArrow");

window.addEventListener("resize", resize);
accountBtn.addEventListener("click", click);

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
    document.removeEventListener("click", anotherClick);
  }
}

function resize() {
  dropDown.className = "";
  mask.className = "";
  icon.innerHTML = "&#xe60b;";
}