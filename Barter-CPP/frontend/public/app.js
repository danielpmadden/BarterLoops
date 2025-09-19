async function fetchOffers() {
  const res = await fetch("http://localhost:8081/api/offers");
  const offers = await res.json();

  const grid = document.getElementById("offersGrid");
  grid.innerHTML = ""; // clear old entries

  offers.forEach(o => {
    const card = document.createElement("div");
    card.className = "bg-white rounded shadow p-4 flex flex-col";
    card.innerHTML = `
      <img src="${o.img}" class="w-full h-40 object-cover rounded mb-2">
      <h3 class="text-lg font-semibold">${o.title}</h3>
      <p class="text-sm text-gray-600 mb-1">${o.desc}</p>
      <p class="text-sm font-medium text-green-600">Wants: ${o.want}</p>
      <span class="text-xs text-gray-400">Posted by ${o.user_id}</span>
    `;
    grid.appendChild(card);
  });
}

// Call on page load
window.onload = fetchOffers;

// Call after submitting
async function submitOffer(e) {
  e.preventDefault();
  const formData = new FormData(e.target);

  // upload file first
  let imgUrl = "";
  if (formData.get("file").size > 0) {
    const uploadRes = await fetch("http://localhost:8081/api/upload", {
      method: "POST",
      body: formData
    });
    const uploadJson = await uploadRes.json();
    imgUrl = uploadJson.url;
  }

  // send offer
  const offer = {
    user_id: formData.get("user_id"),
    title: formData.get("title"),
    desc: formData.get("desc"),
    want: formData.get("want"),
    img: imgUrl
  };
  await fetch("http://localhost:8081/api/offer", {
    method: "POST",
    headers: {"Content-Type":"application/json"},
    body: JSON.stringify(offer)
  });

  // Refresh list
  fetchOffers();
}
