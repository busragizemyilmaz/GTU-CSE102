#include <stdio.h>
#include <stdlib.h>

/* Function to get the current balance from the file */
double get_balance()
{
    double balance;
    FILE * file = fopen("account.txt", "r");

    /* If the file does not exist, set balance to 100.00 by default */
    if (file == NULL)
    {
        balance = 100.00;
    }

    else
    {
        /* Read the balance value from the file */
        fscanf(file, "%lf", &balance);
        fclose(file);
    }

    return balance;
}

/* Function to update the balance in the file */
void update_balance(double new_balance)
{
    FILE * file = fopen("account.txt", "w");

    /* Write the new balance into the file */
    fprintf(file, "%.2f", new_balance);
    fclose(file);
}

/* Function to check and display the current balance */
void check_balance()
{
    double balance = get_balance();
    printf("Current balance: %.2f TRY\n", balance);
}

/* Function to deposit money into the account */
void deposit_money()
{
    double deposit_money_amount;
    double balance = get_balance();
    
    printf("Enter amount to deposit: ");
    scanf("%lf", &deposit_money_amount);

    if (deposit_money_amount >= 0)
    {
        balance += deposit_money_amount;

        printf("Deposit successful! New balance: %.2f TRY\n", balance);

        /* Update the balance after deposit */
        update_balance(balance);
    }

    else
    {
        /* Display a warning if the deposit amount is negative */
        printf("You cannot deposit a negative amount! Please enter a valid value.\n");
    }
}

/* Function to withdraw money from the account */
void withdraw_money()
{
    double withdraw_money_amount;
    double balance = get_balance();

    printf("Enter amount to withdraw: ");
    scanf("%lf", &withdraw_money_amount);

    if (withdraw_money_amount >= 0)
    {
        /* Check if the user has sufficient balance */
        if (withdraw_money_amount <= balance)
        {
            balance -= withdraw_money_amount;

            printf("Withdrawal successful! New balance: %.2f TRY\n", balance);

            /* Update the balance after withdrawal */
            update_balance(balance);
        }
        
        else
        {
            /* Notify the user if the balance is insufficient */
            printf("Insufficient funds! Your balance is only %.2f TRY.\n", balance);
        }
    }

    else
    {
        /* Display a warning if the withdrawal amount is negative */
        printf("You cannot withdraw a negative amount! Please enter a valid value.\n");
    }
}

/* Function to display the menu options */
void menu()
{
    printf("----- Virtual ATM -----\n");
    printf("1. Check Balance\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Exit\n\n");
}

int main()
{
    int choice;

    menu();

    printf("Choose an option: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            check_balance();
            break;
        
        case 2:
            deposit_money();
            break;

        case 3:
            withdraw_money();
            break;

        case 4:
            printf("Thank you for using the program! Goodbye.\n");
            return 0; /* Exit the program */

        default:
            printf("Invalid choice! Please enter a valid option (1,2,3 or 4).\n");
            break;
    }

    return 0;
}
