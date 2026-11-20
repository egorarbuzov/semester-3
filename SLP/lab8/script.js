console.log("Задание 1");

class MediaContent {
  static #createdCount = 0;

  #id;
  #title;
  #duration;
  #filePath;
  #isAvailable;

  constructor(id, title, duration, filePath) {
    if (typeof title !== "string" || title.trim() === "") {
      throw new Error("Название не может быть пустым");
    }

    if (typeof duration !== "number" || duration <= 0) {
      throw new Error("Продолжительность должна быть положительным числом");
    }

    this.#id = id;
    this.#title = title.trim();
    this.#duration = duration;
    this.#filePath = filePath;
    this.#isAvailable = true;

    MediaContent.#createdCount++;
  }

  get id() {
    return this.#id;
  }

  get title() {
    return this.#title;
  }

  get duration() {
    return this.#duration;
  }

  get filePath() {
    return this.#filePath;
  }

  get formattedDuration() {
    const minutes = Math.floor(this.#duration / 60);
    const seconds = this.#duration % 60;
    return `${String(minutes).padStart(2, "0")}:${String(seconds).padStart(2, "0")}`;
  }

  get isAvailable() {
    return this.#isAvailable;
  }

  set isAvailable(value) {
    if (typeof value !== "boolean") {
      throw new Error("Признак доступности должен быть true или false");
    }
    this.#isAvailable = value;
  }

  getInfo() {
    return `${this.#title}, ${this.formattedDuration}`;
  }

  static getCreatedCount() {
    return MediaContent.#createdCount;
  }
}

console.log("Задание 2");

class Track extends MediaContent {
  #artist;
  #album;
  #genre;

  constructor(id, title, duration, filePath, artist, album, genre) {
    super(id, title, duration, filePath);
    this.#artist = artist;
    this.#album = album;
    this.#genre = genre;
  }

  get artist() {
    return this.#artist;
  }

  getInfo() {
    return `Композиция: ${this.title} — ${this.#artist}, ${this.formattedDuration}`;
  }
}

class PodcastEpisode extends MediaContent {
  #author;
  #podcastTitle;
  #episodeNumber;

  constructor(id, title, duration, filePath, author, podcastTitle, episodeNumber) {
    super(id, title, duration, filePath);
    this.#author = author;
    this.#podcastTitle = podcastTitle;
    this.#episodeNumber = episodeNumber;
  }

  get author() {
    return this.#author;
  }

  get podcastTitle() {
    return this.#podcastTitle;
  }

  getInfo() {
    return `Подкаст: ${this.#podcastTitle}, выпуск ${this.#episodeNumber} — ${this.title}`;
  }
}

const track1 = new Track(1, "Night Drive", 205, "audio/night-drive.mp3", "Neon Lights", "Midnight", "Synthwave");
const track2 = new Track(2, "Deep Ocean", 240, "audio/deep-ocean.mp3", "Aurora", "Blue Waves", "Ambient");
const track3 = new Track(3, "Paper Planes", 180, "audio/paper-planes.mp3", "The Kites", "Skyline", "Indie");
const track4 = new Track(4, "Silent Morning", 195, "audio/silent-morning.mp3", "Lumen", "First Light", "Chillout");
const track5 = new Track(5, "City Lights", 232, "audio/city-lights.mp3", "Mono Band", "Urban", "Electro");

const podcast1 = new PodcastEpisode(6, "Как работает браузер", 1900, "audio/web-inside-12.mp3", "Иван Петров", "Web Inside", 12);
const podcast2 = new PodcastEpisode(7, "История JavaScript", 2400, "audio/code-people-5.mp3", "Мария Соколова", "Код и люди", 5);

console.log("Задание 3");

class Playlist {
  #id;
  #title;
  #items = [];

  constructor(id, title) {
    this.#id = id;
    this.#title = title;
  }

  get id() {
    return this.#id;
  }

  get title() {
    return this.#title;
  }

  addItem(item) {
    if (!(item instanceof MediaContent)) {
      throw new Error("Добавлять можно только экземпляры MediaContent");
    }
    if (this.hasItem(item.id)) {
      throw new Error(`Элемент с id ${item.id} уже есть в плейлисте`);
    }
    this.#items.push(item);
  }

  removeItem(id) {
    const index = this.#items.findIndex((item) => item.id === id);
    if (index === -1) {
      throw new Error(`Элемент с id ${id} не найден`);
    }
    this.#items.splice(index, 1);
  }

  findItem(id) {
    return this.#items.find((item) => item.id === id) ?? null;
  }

  hasItem(id) {
    return this.#items.some((item) => item.id === id);
  }

  clear() {
    this.#items = [];
  }

  getTotalDuration() {
    return this.#items.reduce((sum, item) => sum + item.duration, 0);
  }

  get items() {
    return [...this.#items];
  }

  get itemsCount() {
    return this.#items.length;
  }
}

const playlist1 = new Playlist(1, "Любимое");
playlist1.addItem(track1);
playlist1.addItem(track2);
playlist1.addItem(track3);
playlist1.addItem(podcast1);

const playlist2 = new Playlist(2, "Для работы");
playlist2.addItem(track4);
playlist2.addItem(track5);
playlist2.addItem(podcast2);

console.log("Задание 4");

class Player {
  #items = [];
  #currentIndex = 0;
  #volume = 50;
  #isPlaying = false;

  get currentItem() {
    return this.#items[this.#currentIndex] ?? null;
  }

  get status() {
    if (this.#items.length === 0) {
      return `Плейлист не загружен, громкость: ${this.#volume}`;
    }
    const state = this.#isPlaying ? "Играет" : "На паузе";
    return `${state}: ${this.currentItem.getInfo()}, громкость: ${this.#volume}`;
  }

  get volume() {
    return this.#volume;
  }

  set volume(value) {
    if (!Number.isInteger(value) || value < 0 || value > 100) {
      throw new Error(`Громкость должна быть целым числом от 0 до 100, получено: ${value}`);
    }
    this.#volume = value;
  }

  loadPlaylist(playlist) {
    if (!(playlist instanceof Playlist)) {
      throw new Error("Загрузить можно только плейлист");
    }
    this.#items = playlist.items;
    this.#currentIndex = 0;
    this.#isPlaying = false;
  }

  play() {
    if (this.currentItem === null) {
      throw new Error("Плейлист не загружен");
    }
    if (!this.currentItem.isAvailable) {
      throw new Error(`Элемент недоступен: ${this.currentItem.getInfo()}`);
    }
    this.#isPlaying = true;
    console.log(`Воспроизведение: ${this.currentItem.getInfo()}`);
  }

  pause() {
    this.#isPlaying = false;
    console.log("Пауза");
  }

  next() {
    if (this.#items.length === 0) return;
    this.#currentIndex = (this.#currentIndex + 1) % this.#items.length;
  }

  previous() {
    if (this.#items.length === 0) return;
    this.#currentIndex = (this.#currentIndex - 1 + this.#items.length) % this.#items.length;
  }
}

const player = new Player();
player.loadPlaylist(playlist1);
player.volume = 70;
player.play();
player.next();
player.previous();
player.pause();

console.log("Задание 5");

const mediaLibrary = [track1, podcast1, track2, podcast2, track3, track4, track5];
mediaLibrary.forEach((item) => console.log(item.getInfo()));

console.log("Всего создано объектов:", MediaContent.getCreatedCount());