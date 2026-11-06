console.log("Задание 1");
function Product(id, title, price, category) {
    this.id = id;
    this.title = title;
    this.price = (typeof price === "number" && price > 0 && !isNaN(price)) ? price : 0;
    this.category = category;
    this.isAvailable = true;

    this.getInfo = function () {
        return `${this.title}, цена: ${this.price} BYN, категория: ${this.category}, доступен: ${this.isAvailable}`;
    };

    this.changePrice = function (newPrice) {
        if (typeof newPrice === "number" && newPrice > 0 && !isNaN(newPrice)) {
            this.price = newPrice;
        }
    };

    this.applyDiscount = function (percent) {
        if (typeof percent === "number" && percent >= 0 && percent <= 100) {
            this.price = this.price * (1 - percent / 100);
        }
    };

    this.disable = function () {
        this.isAvailable = false;
    };

    this.enable = function () {
        this.isAvailable = true;
    };
}

const product1 = new Product(1, "Ноутбук", 2500, "Техника");
const product2 = new Product(2, "Мышь", 80, "Аксессуары");

console.log(product1.getInfo());

product1.changePrice(2700);
product1.applyDiscount(10);
console.log(product1.getInfo());

product1.disable();
console.log(product1.getInfo());
product1.enable();

console.log("Задание 2");
function ShoppingCart(customerName) {
    this.customerName = customerName;
    this.currency = "BYN";
    this.items = [];
    this.discount = 0;

    this.addItem = function (product, quantity) {
        if (!product.isAvailable || !Number.isInteger(quantity) || quantity <= 0) {
            return;
        }

        let item = this.items.find(item => item.product.id === product.id);

        if (item) {
            item.quantity += quantity;
        } else {
            this.items.push({
                product: { ...product },
                quantity: quantity
            });
        }
    };

    this.removeItem = function (productId) {
        this.items = this.items.filter(item => item.product.id !== productId);
    };

    this.changeQuantity = function (productId, quantity) {
        if (!Number.isInteger(quantity) || quantity <= 0) {
            return;
        }
        let item = this.items.find(item => item.product.id === productId);
        
        if (item) {
            item.quantity = quantity;
        }
    };

    this.setDiscount = function (percent) {
        if (typeof percent === "number" && percent >= 0 && percent <= 100) {
            this.discount = percent;
        }
    };

    this.getTotalBeforeDiscount = function () {
        return this.items.reduce((sum, item) => sum + item.product.price * item.quantity, 0);
    };

    this.getTotal = function () {
        return this.getTotalBeforeDiscount() * (1 - this.discount / 100);
    };

    this.getSummary = function () {
        return this.items.map(item =>
            `${item.product.title}: ${item.quantity} шт. по ${item.product.price} BYN`
        );
    };
}

const cart1 = new ShoppingCart("Михаил");
const cart2 = new ShoppingCart("Алексей");

cart1.addItem(product1, 1);
cart1.addItem(product2, 2);
cart1.addItem(product2, 1);
cart2.addItem(product2, 2);

console.log("Корзина Михаила:", cart1.getSummary());
console.log("Сумма без скидки:", cart1.getTotalBeforeDiscount());

cart1.setDiscount(10);
console.log("Сумма со скидкой:", cart1.getTotal());

cart1.changeQuantity(2, 1);
cart1.removeItem(1);

console.log("Корзина после изменений:", cart1.getSummary());
console.log("Корзина Алексея:", cart2.getSummary());



console.log("Задание 3");
function DeliveryTariff(title, basePrice, pricePerKilogram, maxWeight) {
    this.title = title;
    this.basePrice = basePrice;
    this.pricePerKilogram = pricePerKilogram;
    this.maxWeight = maxWeight;

    this.canDeliver = function (weight) {
        if (typeof weight !== "number" || weight <= 0 || isNaN(weight)) {
            return false;
        }
        return weight <= this.maxWeight;
    };

    this.calculate = function (weight) {
        if (!this.canDeliver(weight)) {
            return null;
        }

        let cost = this.basePrice + weight * this.pricePerKilogram;
        return cost < 0 ? 0 : cost;
    };

    this.getInfo = function (weight) {
        let price = this.calculate(weight);

        if (price === null) {
            return "Доставка невозможна";
        }

        return `${this.title}: ${weight} кг, стоимость ${price} BYN`;
    };
}

const standard = new DeliveryTariff("Стандарт", 5, 2, 20);
const express = new DeliveryTariff("Экспресс", 10, 4, 10);

console.log(standard.getInfo(5));
console.log(express.getInfo(3));
console.log(standard.getInfo(25));

console.log("Задание 4");
const interfaceSettings = {
    _theme: "light",
    _fontSize: 16,
    _language: "ru",

    get theme() {
        return this._theme;
    },

    set theme(value) {
        if (value === "light" || value === "dark") {
            this._theme = value;
        }
    },

    get fontSize() {
        return this._fontSize;
    },

    set fontSize(value) {
        if (Number.isInteger(value) && value >= 12 && value <= 32) {
            this._fontSize = value;
        }
    },

    get language() {
        return this._language;
    },

    set language(value) {
        if (value === "ru" || value === "en" || value === "be") {
            this._language = value;
        }
    },

    get cssClasses() {
        return `theme-${this.theme} font-${this.fontSize} lang-${this.language}`;
    },

    get summary() {
        return `Тема: ${this.theme}, шрифт: ${this.fontSize}, язык: ${this.language}`;
    },

    reset() {
        this.theme = "light";
        this.fontSize = 16;
        this.language = "ru";
    }
};

console.log(interfaceSettings.summary);

interfaceSettings.theme = "dark";
interfaceSettings.fontSize = 20;
interfaceSettings.language = "en";

console.log(interfaceSettings.summary);
console.log(interfaceSettings.cssClasses);

interfaceSettings.theme = "blue";
console.log("После неверной темы:", interfaceSettings.theme);

interfaceSettings.reset();
console.log("После сброса:", interfaceSettings.summary);

console.log("Задание 5");
const player = {
    tracks: [
        { id: 1, title: "Midnight City", artist: "M83" },
        { id: 2, title: "Instant Crush", artist: "Daft Punk" },
        { id: 3, title: "Intro", artist: "The xx" }
    ],

    _currentTrackIndex: 0,
    _volume: 50,
    isPlaying: false,

    get currentTrack() {
        return this.tracks[this._currentTrackIndex];
    },

    get volume() {
        return this._volume;
    },

    set volume(value) {
        if (Number.isInteger(value) && value >= 0 && value <= 100) {
            this._volume = value;
        }
    },

    get status() {
        let state = this.isPlaying ? "Играет" : "На паузе";

        return `${state}: ${this.currentTrack.title} — ${this.currentTrack.artist}, громкость: ${this.volume}`;
    },

    play() {
        this.isPlaying = true;
    },

    pause() {
        this.isPlaying = false;
    },

    next() {
        this._currentTrackIndex =
            (this._currentTrackIndex + 1) % this.tracks.length;
    },

    previous() {
        this._currentTrackIndex =
            (this._currentTrackIndex - 1 + this.tracks.length) % this.tracks.length;
    }
};

console.log("\nЗадание 5");
console.log(player.status);

player.play();
console.log(player.status);

player.next();
console.log(player.status);

player.previous();
console.log(player.status);

player.volume = 80;
console.log(player.status);

player.pause();
console.log(player.status);