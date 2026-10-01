using System;

abstract class BankAccount
{
    private string owner_name;
    private decimal balance;
    private bool isBlocked;
    private DateTime open_date;
    private int pin;

    public int BalanceNumber { get; private set; }
    public string OwnerName => owner_name;
    public DateTime OpenDate => open_date;
    public bool IsBlocked => isBlocked;

    private int failedPinAttempts;
    private const int MaxPinAttempts = 3;

    public decimal Balance
    {
        get { return balance; }
        protected set
        {
            ValidateBalance(value);
            balance = value;
        }
    }

    protected virtual void ValidateBalance(decimal value)
    {
        if (value < 0)
        {
            throw new Exception("Баланс не может быть меньше 0");
        }
    }

    protected BankAccount(int balanceNumber, string ownerName, decimal balance, int pin)
    {
        if (string.IsNullOrWhiteSpace(ownerName))
            throw new Exception("Имя владельца не может быть пустым");

        this.BalanceNumber = balanceNumber;
        this.owner_name = ownerName;
        this.open_date = DateTime.Now;
        this.pin = pin;

        Balance = balance;
    }

    public abstract void Deposit(decimal amount, int pin);
    public abstract void Withdraw(decimal amount, int pin);
    
    protected void CheckPin(int enteredPin)
    {
        if (isBlocked)
            throw new Exception("Счёт заблокирован. Обратитесь в банк.");

        if (enteredPin == pin)
        {
            failedPinAttempts = 0;
            return;
        }

        failedPinAttempts++;
        if (failedPinAttempts >= MaxPinAttempts)
        {
            isBlocked = true;
            throw new Exception("Неверный PIN 3 раза. Счёт заблокирован!");
        }

        throw new Exception($"Неверный PIN. Осталось попыток: {MaxPinAttempts - failedPinAttempts}");
    }
}

class SavingsAccount : BankAccount
{
    private decimal ratePerMonth;

    public SavingsAccount(int number, string ownerName, decimal balance, int pin, decimal ratePerMonth)
        : base(number, ownerName, balance, pin)
    {
        if (ratePerMonth < 0)
            throw new Exception("Ставка не может быть отрицательной");
        this.ratePerMonth = ratePerMonth;
    }

    public override void Deposit(decimal amount, int pin)
    {
        CheckPin(pin);
        if (amount <= 0)
            throw new Exception("Сумма должна быть больше 0");
        Balance += amount;
    }

    public override void Withdraw(decimal amount, int pin)
    {
        CheckPin(pin);
        if (amount <= 0)
            throw new Exception("Сумма должна быть больше 0");
        if (amount > Balance)
            throw new Exception("Недостаточно средств");
        Balance -= amount;
    }
    
    public void AccrueInterest(int months, int pin)
    {
        CheckPin(pin);
        if (months <= 0)
            throw new Exception("Месяцев должно быть больше 0");

        for (int i = 0; i < months; i++)
            Balance += Balance * ratePerMonth;
    }
}

class CheckingAccount : BankAccount
{
    private decimal operationCost;

    public CheckingAccount(int number, string ownerName, decimal balance, int pin, decimal operationCost)
        : base(number, ownerName, balance, pin)
    {
        if (operationCost < 0)
            throw new Exception("Комиссия не может быть отрицательной");
        this.operationCost = operationCost;
    }

    public override void Deposit(decimal amount, int pin)
    {
        CheckPin(pin);
        if (amount <= 0)
            throw new Exception("Сумма должна быть больше 0");
        Balance += amount - operationCost;
    }

    public override void Withdraw(decimal amount, int pin)
    {
        CheckPin(pin);
        if (amount <= 0)
            throw new Exception("Сумма должна быть больше 0");

        decimal total = amount + operationCost;
        if (total > Balance)
            throw new Exception("Недостаточно средств с учётом комиссии");

        Balance -= total;
    }
}

class CreditAccount : BankAccount
{
    public decimal CreditLimit { get; }
    public decimal RatePerMonth { get; }

    public CreditAccount(int number, string ownerName, decimal balance, int pin,
                         decimal creditLimit, decimal ratePerMonth)
        : base(number, ownerName, balance, pin)
    {
        if (creditLimit <= 0)
            throw new Exception("Лимит должен быть больше 0");
        if (ratePerMonth < 0)
            throw new Exception("Ставка не может быть отрицательной");

        CreditLimit = creditLimit;
        RatePerMonth = ratePerMonth;
    }

    // у кредитного баланс МОЖЕТ быть отрицательным (долг)
    protected override void ValidateBalance(decimal value)
    {
        if (value < -CreditLimit)
            throw new Exception($"Превышен кредитный лимит ({CreditLimit:F2})");
    }

    public override void Deposit(decimal amount, int pin)
    {
        CheckPin(pin);
        if (amount <= 0)
            throw new Exception("Сумма должна быть больше 0");
        Balance += amount;
    }

    public override void Withdraw(decimal amount, int pin)
    {
        CheckPin(pin);
        if (amount <= 0)
            throw new Exception("Сумма должна быть больше 0");
        Balance -= amount;
    }

    public decimal Debt => Balance < 0 ? -Balance : 0;

    public void AccrueInterest(int months, int pin)
    {
        CheckPin(pin);
        if (months <= 0)
            throw new Exception("Месяцев должно быть больше 0");
        if (Debt == 0)
        {
            Console.WriteLine("Долга нет — проценты не начисляются.");
            return;
        }

        for (int i = 0; i < months; i++)
            Balance -= Debt * RatePerMonth;
    }
}

// ---------- Программа ----------
class Program
{
    static void Main()
    {
        var savings = new SavingsAccount(1001, "Иван", 10000m, 1234, 0.01m);
        var checking = new CheckingAccount(1002, "Пётр", 5000m, 2222, 50m);
        var credit = new CreditAccount(1003, "Анна", 0m, 3333, 100000m, 0.02m);

        Console.WriteLine($"Сберегательный №{savings.BalanceNumber}, баланс: {savings.Balance}");
        savings.Deposit(5000m, 1234);
        Console.WriteLine($"После пополнения: {savings.Balance}");

        savings.AccrueInterest(3, 1234);
        Console.WriteLine($"После 3 месяцев процентов: {savings.Balance}");

        Console.WriteLine($"\nРасчётный №{checking.BalanceNumber}");
        checking.Withdraw(1000m, 2222);
        Console.WriteLine($"После снятия 1000 (комиссия 50): {checking.Balance}");

        Console.WriteLine($"\nКредитный №{credit.BalanceNumber}");
        credit.Withdraw(20000m, 3333);
        Console.WriteLine($"После снятия 20000: баланс {credit.Balance}, долг {credit.Debt}");

        credit.AccrueInterest(2, 3333);
        Console.WriteLine($"После 2 месяцев процентов: баланс {credit.Balance}, долг {credit.Debt}");
        
        Console.WriteLine("\nПробуем неверный PIN 3 раза...");
        for (int i = 1; i <= 3; i++)
        {
            try { checking.Withdraw(100m, 9999); }
            catch (Exception ex) { Console.WriteLine($"  Попытка {i}: {ex.Message}"); }
        }

        Console.WriteLine($"Счёт заблокирован: {checking.IsBlocked}");
    }
}