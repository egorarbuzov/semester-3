function formatDuration(seconds) {
    const minutes = Math.floor(seconds / 60);
    const rest = seconds % 60;
    return `${minutes}:${String(rest).padStart(2, "0")}`;
}

class MediaContent {
    constructor(id, title, duration, filePath) {
        this.id = id;
        this.title = title;
        this.duration = duration;
        this.filePath = filePath;
    }

    get formattedDuration() {
        return formatDuration(this.duration);
    }
}

class Track extends MediaContent {
    constructor(id, title, duration, filePath, artist, album, genre, cover = "") {
        super(id, title, duration, filePath);
        this.artist = artist;
        this.album = album;
        this.genre = genre;
        this.cover = cover;
    }
}

class Playlist {
    constructor(id, title) {
        this.id = id;
        this.title = title;
        this.items = [];
    }

    addItem(track) {
        this.items.push(track);
    }

    getTotalDuration() {
        return this.items.reduce((sum, track) => sum + track.duration, 0);
    }
}

const cover = "data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='120' height='120'%3E%3Crect width='120' height='120' fill='%23888'/%3E%3C/svg%3E";

const tracks = [
    new Track(1, "Night Drive", 205, "night-drive.mp3", "Neon Lights", "Midnight", "Synthwave", cover),
    new Track(2, "Deep Ocean", 240, "deep-ocean.mp3", "Aurora", "Blue Waves", "Ambient", cover),
    new Track(3, "Paper Planes", 180, "paper-planes.mp3", "The Kites", "Skyline", "Indie", cover),
    new Track(4, "Silent Morning", 195, "silent-morning.mp3", "Lumen", "First Light", "Chillout", cover),
    new Track(5, "City Lights", 232, "city-lights.mp3", "Mono Band", "Urban", "Electro", cover),
    new Track(6, "Rainy Window", 210, "rainy-window.mp3", "Lo-Fi Cat", "", "Lo-Fi")
];

const playlist = new Playlist(1, "Мой плейлист");
playlist.addItem(tracks[0]);
playlist.addItem(tracks[2]);

const catalogElement = document.querySelector("#catalog");
const catalogCountElement = document.querySelector("#catalog-count");
const playlistElement = document.querySelector("#playlist");
const playlistCountElement = document.querySelector("#playlist-count");
const playlistDurationElement = document.querySelector("#playlist-duration");

function createTrackCard(track) {
    const card = document.createElement("article");
    card.dataset.trackId = track.id;

    if (track.cover !== "") {
        const image = document.createElement("img");
        image.src = track.cover;
        image.alt = track.title;
        card.append(image);
    } else {
        const imagePlaceholder = document.createElement("p");
        imagePlaceholder.textContent = "Обложка отсутствует";
        card.append(imagePlaceholder);
    }

    const title = document.createElement("h3");
    title.textContent = track.title
    card.append(title);

    const artist = document.createElement("p");
    artist.textContent = `Исполнитель: ${track.artist}`;
    card.append(artist);

    const album = document.createElement("p");
    album.textContent = track.album === "" ? "Альбом не указан" : `Альбом: ${track.album}`;
    card.append(album);

    const genre = document.createElement("p");
    genre.textContent = `Жанр: ${track.genre}`;
    card.append(genre);

    const duration = document.createElement("p");
    duration.textContent = `Продолжительность: ${track.formattedDuration}`;
    card.append(duration);

    const playButton = document.createElement("button");
    playButton.type = "button";
    playButton.textContent = "Воспроизвести";
    card.append(playButton);

    const addButton = document.createElement("button");
    addButton.type = "button";
    addButton.textContent = "Добавить в плейлист";
    card.append(addButton);

    return card;
}

function renderCatalog(tracks) {
    catalogElement.replaceChildren();

    const fragment = document.createDocumentFragment();

    if (tracks.length === 0) {
        const message = document.createElement("p");
        message.textContent = "В каталоге нет композиций";
        fragment.append(message);
    } else {
        tracks.forEach((track) => {
            fragment.append(createTrackCard(track));
        });
    }

    catalogElement.append(fragment);
    catalogCountElement.textContent = `Композиций: ${tracks.length}`;
}

function renderPlaylist(playlist) {
    playlistElement.replaceChildren();

    const fragment = document.createDocumentFragment();

    if (playlist.items.length === 0) {
        const message = document.createElement("li");
        message.textContent = "В плейлисте пока нет композиций";
        fragment.append(message);
    } else {
        playlist.items.forEach((track, index) => {
            const item = document.createElement("li");
            item.dataset.trackId = track.id;
            item.textContent = `${index + 1}. ${track.title} — ${track.artist}, ${track.formattedDuration} `;

            const removeButton = document.createElement("button");
            removeButton.type = "button";
            removeButton.textContent = "Удалить";
            item.append(removeButton);

            fragment.append(item);
        });
    }

    playlistElement.append(fragment);
    playlistCountElement.textContent = `Композиций: ${playlist.items.length}`;
    playlistDurationElement.textContent = `Общая продолжительность: ${formatDuration(playlist.getTotalDuration())}`;
}

renderCatalog(tracks);
renderPlaylist(playlist);
