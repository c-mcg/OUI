

const numResults = getNumberOfResults();
const items = getItemsOnPage();
const numUnpricedItems = 75 - items.length;

const output = {
    numResults,
    numUnpricedItems,
    items,
}

console.log(output);

function getNumberOfResults() {
    const breadcrumbs = document.querySelector(".breadcrumbs");
    const numResultsElement = breadcrumbs.nextElementSibling.nextElementSibling.nextElementSibling;
    return Number(numResultsElement.firstElementChild.textContent);
}

function getItemsOnPage() {
    const priceElements = document.querySelectorAll(".price-history-link");
    return [...priceElements].map((priceElement) => {
        return {
            name: getNameFromPriceElement(priceElement),
            ...getPriceFromPriceElement(priceElement)
        };
    });
}

function getPriceFromPriceElement(priceElement) {
    let priceText = priceElement.textContent;
    const parts = priceText.split(" ");

    let price = Number(parts[0].split(",").join(""));
    let currency = parts[1];
    let inflationNotice = false;
    if (isNaN(price)) {
        price = 0;
        currency = "";
        inflationNotice = priceText === "Inflation Notice";
    }

    return {
        price,
        currency,
        inflationNotice,
        priceText,
     };
}

function getNameFromPriceElement(priceElement) {
    try {
        const nameElement = priceElement.parentElement.previousSibling.previousSibling.previousSibling;
        return nameElement.textContent
    } catch (e) {
        return null;
    }
}