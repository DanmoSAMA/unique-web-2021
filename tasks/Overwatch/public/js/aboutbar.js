const bar = document.getElementById("about-bar");
document.addEventListener("scroll", ceiling);

function ceiling() {
  if (document.documentElement.scrollTop > 40) {
    bar.className = "fixed";
  } else {
    bar.className = "";
  }
}
