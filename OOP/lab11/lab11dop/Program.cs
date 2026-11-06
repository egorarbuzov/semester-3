using System;

class GameStartedEventArgs : EventArgs
{
    public int RangeMin { get; set; }
    public int RangeMax { get; set; }
    public GameStartedEventArgs(int min, int max)
    {
        RangeMin = min;
        RangeMax = max;
    }
}

class NumberGuessedEventArgs : EventArgs
{
    public int Number { get; set; }
    public int Attempts { get; set; }
    public NumberGuessedEventArgs(int number, int attempts)
    {
        Number = number;
        Attempts = attempts;
    }
}

class GameOverEventArgs : EventArgs
{
    public bool IsWin { get; set; }
    public int Attempts { get; set; }
    public GameOverEventArgs(bool isWin, int attempts)
    {
        IsWin = isWin;
        Attempts = attempts;
    }
}

class GuessingGame
{
    public event EventHandler<GameStartedEventArgs> GameStarted;
    public event EventHandler<NumberGuessedEventArgs> NumberGuessed;
    public event EventHandler<GameOverEventArgs> GameOver;

    private readonly int _min;
    private readonly int _max;
    private readonly int _maxAttempts;

    public GuessingGame(int min, int max, int maxAttempts = 7)
    {
        _min = min;
        _max = max;
        _maxAttempts = maxAttempts;
    }
    
    public void Start(Func<int, int> hintProvider)
    {
        GameStarted?.Invoke(this, new GameStartedEventArgs(_min, _max));

        int low = _min;
        int high = _max;
        int attempts = 0;
        bool guessed = false;
        int secret = -1;

        while (attempts < _maxAttempts)
        {
            attempts++;
            int guess = (low + high) / 2;   
            int hint = hintProvider(guess);

            if (hint == 0)
            {
                secret = guess;
                guessed = true;
                NumberGuessed?.Invoke(this, new NumberGuessedEventArgs(guess, attempts));
                break;
            }
            else if (hint < 0) high = guess - 1; 
            else               low = guess + 1; 
        }
        
        GameOver?.Invoke(this, new GameOverEventArgs(guessed, attempts));
    }
}

class Statistics
{
    public void OnGameStarted(object sender, GameStartedEventArgs e)
    {
        Console.WriteLine($"Игра началась. Диапазон: [{e.RangeMin}; {e.RangeMax}]");
    }
    public void OnNumberGuessed(object sender, NumberGuessedEventArgs e)
    {
        Console.WriteLine($"Число {e.Number} угадано за {e.Attempts} попыток.");
    }
    public void OnGameOver(object sender, GameOverEventArgs e)
    {
        string result = e.IsWin ? "ПОБЕДА" : "ПОРАЖЕНИЕ";
        Console.WriteLine($"Итог: {result}, попыток: {e.Attempts}");
    }
}

// Подписчик 2: лог
class Logger
{
    public void OnGameStarted(object sender, GameStartedEventArgs e)
    {
        Console.WriteLine($"{DateTime.Now:HH:mm:ss} — старт игры");
    }
    public void OnNumberGuessed(object sender, NumberGuessedEventArgs e)
    {
        Console.WriteLine($"{DateTime.Now:HH:mm:ss} — угадано {e.Number}");
    }
    public void OnGameOver(object sender, GameOverEventArgs e)
    {
        Console.WriteLine($"{DateTime.Now:HH:mm:ss} — игра завершена (win={e.IsWin})");
    }
}

class HintProvider
{
    public void OnGameStarted(object sender, GameStartedEventArgs e)
    {
        Console.WriteLine($"Загадайте число от {e.RangeMin} до {e.RangeMax}. Я буду угадывать.");
    }
    public void OnNumberGuessed(object sender, NumberGuessedEventArgs e)
    {
        Console.WriteLine($"Угадал! Ваше число — {e.Number}. Попыток: {e.Attempts}");
    }
    public void OnGameOver(object sender, GameOverEventArgs e)
    {
        if (!e.IsWin)
            Console.WriteLine("Не угадал за отведённые попытки. В следующий раз повезёт!");
    }
}

class Program
{
    static void Main()
    {
        Console.WriteLine("Игра «Угадай число» с событиями\n");

        int secret = Convert.ToInt32(Console.ReadLine());
        Console.WriteLine($"(Отладка: загадано число {secret})\n");

        var game = new GuessingGame(1, 100);
        var stats = new Statistics();
        var logger = new Logger();
        var hints = new HintProvider();

        game.GameStarted += stats.OnGameStarted;
        game.GameStarted += logger.OnGameStarted;
        game.GameStarted += hints.OnGameStarted;

        game.NumberGuessed += stats.OnNumberGuessed;
        game.NumberGuessed += logger.OnNumberGuessed;
        game.NumberGuessed += hints.OnNumberGuessed;

        game.GameOver += stats.OnGameOver;
        game.GameOver += logger.OnGameOver;
        game.GameOver += hints.OnGameOver;
        
        Func<int, int> hintProvider = guess =>
        {
            Console.WriteLine($"  Компьютер предполагает: {guess}");
            if (guess < secret) { Console.WriteLine("  → загаданное БОЛЬШЕ"); return 1; }
            if (guess > secret) { Console.WriteLine("  → загаданное МЕНЬШЕ"); return -1; }
            Console.WriteLine("  → УГАДАНО!");
            return 0;
        };

        game.Start(hintProvider);

        // Отписка одного подписчика (демонстрация -=)
        Console.WriteLine("\nОтписка логгера...");
        game.GameStarted   -= logger.OnGameStarted;
        game.NumberGuessed -= logger.OnNumberGuessed;
        game.GameOver      -= logger.OnGameOver;

        Console.WriteLine("\nПовторная игра (логгер не выводит сообщений):\n");
        game.Start(hintProvider);
    }
}