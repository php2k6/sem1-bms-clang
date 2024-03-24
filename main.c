#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <sys/stat.h>
#include<sys/types.h>
#include <direct.h>


struct newUser{
    char name[100];
    char email[100];
    long long pn;   //10 digits
    char address[300];
    char pan_card[11];
    long long aadhar;   //12 digits
    char mpin[7];   //6 digits
    int ini_dep;
    int acc_num;    //5 digits
};


struct currentUser{
    char name[100];
    long long pn;
    int acc_num;
    char mpin[7];
};

void printMainMenu();
void printUserMenu();
void printPayeeManagerMenu();
struct newUser signUp();
struct currentUser login(int acc_num, char mpin[]);
void printUserDetails(struct newUser user);
void createMainFile();  // account_number|phone_number|name|mpin
void createUserFiles(struct newUser user);  //bank_account number foler --> 1)accountdetails.txt 2)user_statement.txt 3)payee.txt
void addUserRecord(struct newUser user);  //1)accountdetails.txt --> acc_num|name|email|phone|address|pan|aadhar|intial_Deposit|mpin 
                                          //2)user_statement --> first line- balance following lines- statement
void printCurrentUserDetails(struct currentUser user);
void addUserData(struct newUser temp);
void checkStatement(int bank_acc_num);
int balanceCheck(int bank_account, int amount);
int viewPayees(int bank_acc_num);
int payeeCheck(int acc_num);
void addPayee(int acc_num, char name[], int p_acc_num);
void removePayee(int bank_acc_num, int payee_acc_num);
void viewAccountInformation(int bank_acc_num);
void deleteAccount(int bank_acc_num);
void withdrawMoney(int bank_acc_num);
void depositMoney(int bank_acc_num);
void addEntry(int bank_acc_num, int amount, char remarks[], int type);
void fundTransfer(int bank_acc_num);

void main(){
    int flag = 1, ch;
    createMainFile();
    
    while(flag){
        printMainMenu();
        scanf("%d",&ch);
        if(ch==1){
            int login_acc;
            char login_mpin[7];
            struct currentUser loggedInUser;
            printf("Enter Bank Account Number (5 digits): ");
            scanf("%d", &login_acc);
            printf("Enter MPIN: ");
            scanf(" %6[0-9]", login_mpin);
            loggedInUser = login(login_acc, login_mpin);
            if(loggedInUser.acc_num == 0){
                printf("Invalid Credentials!Please Try again.\n");
            }else{
                printf("Successful Login!\n");
                int flag1=1;
                while(flag1){
                    int ch1;
                    printUserMenu();
                    scanf("%d",&ch1);
                    if(ch1==1){
                        checkStatement(loggedInUser.acc_num);
                        printf("Enter any character to return back: \n");
                        getch();
                    }else if(ch1==2){
                        fundTransfer(loggedInUser.acc_num);
                        printf("Enter any character to return back: \n");
                        getch();

                    }
                    else if(ch1==3){
                        withdrawMoney(loggedInUser.acc_num);
                        printf("Enter any character to return back: \n");
                        getch();

                    }else if (ch1 == 4) {
                        depositMoney(loggedInUser.acc_num);
                        printf("Enter any character to return back: \n");
                        getch();

                    }else if(ch1==5){
                        int ch2,flag2=1;
                        while(flag2){
                            printPayeeManagerMenu();
                            scanf("%d",&ch2);
                            if(ch2==1){
                                char p1[100];
                                int p2;
                                printf("Enter the Name of the Payee: ");
                                scanf("%[\n]");
                                scanf("%[^\n]",p1);
                                printf("Enter the Bank Account Number of the Payee: ");
                                scanf("%d",&p2);
                                if(payeeCheck(p2)){
                                    addPayee(loggedInUser.acc_num, p1, p2);
                                }else{
                                    printf("Payee with Bank Account Number %d not found.\n",p2);
                                }
                                

                            }else if(ch2==2){
                                int a;
                                viewPayees(loggedInUser.acc_num);
                                printf("Enter any character to return back: \n");
                                getch();
                                
                            }else if(ch2==3){
                                int p1;
                                printf("Enter Bank Account Number of the Payee to be removed: ");
                                scanf("%d",&p1);
                                removePayee(loggedInUser.acc_num, p1);

                            }else if(ch2==4){
                                flag2=0;
                                
                            }else{
                                printf("Invalid Input!Please try again.\n");
                            }     
                        }              
                    }else if(ch1==6){
                        viewAccountInformation(loggedInUser.acc_num);
                        printf("Enter any character to return back: \n");
                        getch();
                    }else if(ch1==7){
                        char empin[7];
                        printf("Enter MPIN for confirmation: ");
                        scanf("%[\n]");
                        scanf("%6[0-9]", empin);
                        if(strcmp(empin, loggedInUser.mpin)==0){
                            deleteAccount(loggedInUser.acc_num);
                            flag1 = 0;
                        }else{
                            printf("Invalid MPIN!Aborting Deletion.\n");
                        }
                    }else if(ch1==8){
                        printf("Successfully Logged Out.\n");
                        flag1=0;
                    }
                    else {
                        printf("Invalid Input!Please Try again.\n");
                    }
                }
            }
        }else if(ch==2){
            struct newUser temp;
            srand(time(NULL));
            temp = signUp();
            addUserRecord(temp);
            createUserFiles(temp);
            addUserData(temp);
            printf("User Successfully Created!\n");
            printf("Your Bank Account Number is %d!Kindly use it to Log In.\n",temp.acc_num);



        }else if(ch==3){
            flag=0;
        }else{
            printf("Invalid Input! Try again\n");
        }

    
    }
}

void printMainMenu() {
    printf("=====================================\n");
    printf("|          WELCOME TO BMS!           |\n");
    printf("=====================================\n");
    printf("| 1. Existing User? Login            |\n");
    printf("| 2. New User? Sign Up               |\n");
    printf("| 3. Exit                            |\n");
    printf("=====================================\n");
    printf("Enter your choice: ");
}

void printUserMenu() {
    printf("=====================================\n");
    printf("|             MAIN MENU              |\n");
    printf("=====================================\n");
    printf("| 1. Check Bank Account Statement    |\n");
    printf("| 2. Fund Transfer                   |\n");
    printf("| 3. Withdraw Money                  |\n");
    printf("| 4. Deposit Money                   |\n");
    printf("| 5. Manage Payees                   |\n");
    printf("| 6. View Account Information        |\n");
    printf("| 7. Close Account                   |\n");
    printf("| 8. Log Out                         |\n");
    printf("=====================================\n");
    printf("Enter your choice: ");
}

void printPayeeManagerMenu() {
    printf("=====================================\n");
    printf("|           PAYEE MANAGER            |\n");
    printf("=====================================\n");
    printf("| 1. Add Payee                       |\n");
    printf("| 2. View All Payees                 |\n");
    printf("| 3. Remove Payee                    |\n");
    printf("| 4. Exit                            |\n");
    printf("=====================================\n");
    printf("Enter your choice: ");
}

struct newUser signUp() {
    struct newUser user;

    printf("Enter Name: ");
    scanf(" %[^\n]", user.name);

    printf("Enter Email: ");
    scanf(" %[^\n]", user.email);

    printf("Enter Phone Number(10 digits): ");
    scanf("%lld", &user.pn);

    printf("Enter Address: ");
    scanf(" %[^\n]", user.address);

    printf("Enter PAN Card(10 Characters): ");
    scanf(" %[^\n]", user.pan_card);

    printf("Enter Aadhar Number(12 Digits): ");
    scanf("%lld", &user.aadhar);

    printf("Enter MPIN(6 digits): ");
    scanf(" %6[0-9]", user.mpin); // Read up to 6 characters (excluding null terminator)

    printf("Enter Initial Deposit(Minimum Rs 1000): ");
    scanf("%d", &user.ini_dep);

    user.acc_num = rand() % 90000 + 10000; // 5 digit generator

    return user;
}

void printUserDetails(struct newUser user) {
    printf("\n \n \n \n");
    printf("Name: %s\n", user.name);
    printf("Email: %s\n", user.email);
    printf("Phone Number: %lld\n", user.pn);
    printf("Address: %s\n", user.address);
    printf("PAN Card: %s\n", user.pan_card);
    printf("Aadhar Number: %lld\n", user.aadhar);
    printf("MPIN: %s\n", user.mpin);
    printf("Initial Deposit: %d\n", user.ini_dep);
    printf("Account Number: %d\n", user.acc_num);
}

void createMainFile() {
    FILE *userRecord;
    if ((userRecord = fopen("userrecord.txt", "r")) != NULL) {
        fclose(userRecord);
        return;
    }
    userRecord = fopen("userrecord.txt", "w");
    if (userRecord == NULL) {
        printf("Error creating userrecord.txt\n");
        exit(EXIT_FAILURE);
    }

    fclose(userRecord);
}

void createUserFiles(struct newUser user){
    char folderName[6];
    char fileName[30];

    sprintf(folderName, "%d", user.acc_num);
    mkdir(folderName);

    sprintf(fileName, "%s/account_details.txt", folderName);
    FILE *accountDetailsFile = fopen(fileName, "w");
    if (accountDetailsFile == NULL) {
        printf("Error creating account_details.txt\n");
        exit(EXIT_FAILURE);
    }
    fclose(accountDetailsFile);

    sprintf(fileName, "%s/user_statement.txt", folderName);
    FILE *statementFile = fopen(fileName, "w");
    if (statementFile == NULL) {
        printf("Error creating user_statement.txt\n");
        exit(EXIT_FAILURE);
    }
    fclose(statementFile);

    sprintf(fileName, "%s/payees.txt", folderName);
    FILE *payeesFile = fopen(fileName, "w");
    if (payeesFile == NULL) {
        printf("Error creating payees.txt\n");
        exit(EXIT_FAILURE);
    }
    fclose(payeesFile);

}

void addUserRecord(struct newUser user) {
    FILE *userRecord = fopen("userrecord.txt", "a");
    if (userRecord == NULL) {
        printf("Error opening userrecord.txt for appending\n");
        exit(EXIT_FAILURE);
    }
    fprintf(userRecord, "%d|%lld|%s|%s\n", user.acc_num, user.pn, user.name, user.mpin);

    fclose(userRecord);
}

struct currentUser login(int acc_num, char mpin[]) {
    struct currentUser loggedInUser;

    FILE *userRecord = fopen("userrecord.txt", "r");
    if (userRecord == NULL) {
        printf("Error opening userrecord.txt for reading\n");
        exit(EXIT_FAILURE);
    }

    int found = 0;
    while (fscanf(userRecord, "%d|%lld|%99[^|]|%6[0-9]", &loggedInUser.acc_num, &loggedInUser.pn, loggedInUser.name, loggedInUser.mpin) == 4) {

        if (loggedInUser.acc_num == acc_num && strcmp(loggedInUser.mpin, mpin) == 0) {
            found = 1;
            break;
        }
    }
    fclose(userRecord);

    if (found) {
        return loggedInUser;
    } else {
        struct currentUser invalidUser = { "", 0, 0, "" };
        return invalidUser;
    }
}

void printCurrentUserDetails(struct currentUser user){
    printf("Account Number: %d\n", user.acc_num);
    printf("Name: %s\n", user.name);
    printf("Phone Number: %lld\n", user.pn);
    printf("MPIN: %s\n", user.mpin);
}

void addUserData(struct newUser temp) {
    char folderName[6];
    char fileName[30];

    sprintf(folderName, "%d", temp.acc_num);

    sprintf(fileName, "%s/account_details.txt", folderName);
    FILE *accountDetailsFile = fopen(fileName, "a");
    if (accountDetailsFile == NULL) {
        printf("Error opening account_details.txt for appending\n");
        exit(EXIT_FAILURE);
    }
    fprintf(accountDetailsFile, "%d|%s|%s|%lld|%s|%s|%lld|%d|%s\n", temp.acc_num, temp.name, temp.email, temp.pn, temp.address, temp.pan_card, temp.aadhar, temp.ini_dep, temp.mpin);
    fclose(accountDetailsFile);

    sprintf(fileName, "%s/user_statement.txt", folderName);
    FILE *statementFile = fopen(fileName, "w");
    if (statementFile == NULL) {
        printf("Error opening user_statement.txt for writing\n");
        exit(EXIT_FAILURE);
    }
    fprintf(statementFile, "%d\n", temp.ini_dep); 
    fclose(statementFile);

}

void checkStatement(int bank_acc_num) {
    char folderName[6];
    char fileName[30];

    sprintf(folderName, "%d", bank_acc_num);
    sprintf(fileName, "%s/user_statement.txt", folderName);

    FILE *statementFile = fopen(fileName, "r");
    if (statementFile == NULL) {
        printf("Error opening user_statement.txt for reading\n");
        exit(EXIT_FAILURE);
    }

    int currentBalance;
    fscanf(statementFile, "%d", &currentBalance);
    printf("==========================================================================\n");
    printf("|Current Balance: %-56d|\n", currentBalance);
    printf("==========================================================================\n");
    printf("|%-10s | %-10s | %-4s | %-40s|\n", "date", "amount", "type", "remarks");
    printf("==========================================================================\n");


    while (fgetc(statementFile) != '\n');

    // Read and print each entry
    char date[15], amount[10], type[5], remarks[50];
    int entry = 0;
    while (fscanf(statementFile, "%14[^|]|%9[^|]|%4[^|]|%49[^\n]\n", date, amount, type, remarks) == 4) {
        printf("|%-10s | %-10s | %-4s | %-40s|\n", date, amount, type, remarks);
        entry++;
    }


    if (!entry)
        printf("|No entries in the Statement!                                             |\n");
    printf("==========================================================================\n");
    fclose(statementFile);
}

int balanceCheck(int bank_account, int amount) {
    char fileName[50];
    sprintf(fileName, "%d/user_statement.txt", bank_account);

    FILE *statementFile = fopen(fileName, "r");
    if (statementFile == NULL) {
        printf("Error opening user_statement.txt for reading\n");
        exit(EXIT_FAILURE);
    }

    // Read the current balance from the first line
    int currentBalance;
    fscanf(statementFile, "%d", &currentBalance);

    fclose(statementFile);

    // Return 1 if amount is greater than the balance, 0 otherwise
    return (amount > currentBalance) ? 1 : 0;
}

int viewPayees(int bank_acc_num) {
    char folderName[6];
    char fileName[30];

    sprintf(folderName, "%d", bank_acc_num);
    sprintf(fileName, "%s/payees.txt", folderName);

    FILE *payeesFile = fopen(fileName, "r");
    if (payeesFile == NULL) {
        printf("Error opening payees.txt for reading\n");
        exit(EXIT_FAILURE);
    }
    printf("=====================================\n");
    printf("|            Payee List              |\n");
    printf("=====================================\n");
    printf("|%s| %-5s | %-24s |\n","S","AccNo","Name");
    printf("=====================================\n");
    int entryCount = 0;
    while (fscanf(payeesFile, "%d|%[^\n]\n", &bank_acc_num, fileName) == 2) {
        entryCount++;
        printf("| %d | %-5d | %-22s |\n", entryCount, bank_acc_num, fileName);
    }
    if (entryCount == 0) {
        printf("|No payees found for this account    |.\n");\
        
    }
    printf("=====================================\n");
    fclose(payeesFile);
    return entryCount;
}

int payeeCheck(int acc_num) {
    FILE *userRecord = fopen("userrecord.txt", "r");
    if (userRecord == NULL) {
        printf("Error opening userrecord.txt for reading\n");
        exit(EXIT_FAILURE);
    }

    int found = 0;
    int temp;
    while (fscanf(userRecord, "%d|%*[^\n]\n", &temp) == 1) {
        if (temp == acc_num) {
            found = 1;
            break;
        }
    }

    fclose(userRecord);

    return found;
}

void addPayee(int acc_num, char name[], int p_acc_num) {
    char folderName[6];
    char fileName[30];

    sprintf(folderName, "%d", acc_num);
    sprintf(fileName, "%s/payees.txt", folderName);

    if (acc_num != p_acc_num) {
        FILE *payeesFile = fopen(fileName, "a");
        if (payeesFile == NULL) {
            printf("Error opening payees.txt for appending\n");
            exit(EXIT_FAILURE);
        }

        fprintf(payeesFile, "%d|%s\n", p_acc_num, name);

        fclose(payeesFile);

        printf("Payee added successfully!\n");
    } else {
        printf("Cannot add your own account as a payee.\n");
    }
}

void removePayee(int bank_acc_num, int payee_acc_num) {
    char folderName[6];
    char fileName[30];

    sprintf(folderName, "%d", bank_acc_num);
    sprintf(fileName, "%s/payees.txt", folderName);

    FILE *payeesFile = fopen(fileName, "r");
    if (payeesFile == NULL) {
        printf("Error opening payees.txt for reading\n");
        exit(EXIT_FAILURE);
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file\n");
        fclose(payeesFile);
        exit(EXIT_FAILURE);
    }

    int found = 0;
    char line[256];
    while (fgets(line, sizeof(line), payeesFile) != NULL) {
        int currentPayeeAccNum;
        char payeeName[100];
        if (sscanf(line, "%d|%[^|\n]", &currentPayeeAccNum, payeeName) == 2) {
            if (currentPayeeAccNum == payee_acc_num) {
                found = 1;
                continue;
            }
            fprintf(tempFile, "%d|%s\n", currentPayeeAccNum, payeeName);
        }
    }

    fclose(payeesFile);
    fclose(tempFile);

    remove(fileName);
    rename("temp.txt", fileName);

    if (!found) {
        printf("Payee with Account Number %d not found for Account Number %d.\n", payee_acc_num, bank_acc_num);
    } else {
        printf("Payee with Account Number %d removed successfully from Account Number %d.\n", payee_acc_num, bank_acc_num);
    }
}

void viewAccountInformation(int bank_acc_num) {
    char folderName[6];
    char fileName[30];

    sprintf(folderName, "%d", bank_acc_num);
    sprintf(fileName, "%s/account_details.txt", folderName);

    FILE *accountDetailsFile = fopen(fileName, "r");
    if (accountDetailsFile == NULL) {
        printf("Error opening account_details.txt for reading\n");
        exit(EXIT_FAILURE);
    }
    printf("=======================================================================================================================\n");
    printf("|                                                  Account Details                                                    |\n");
    printf("=======================================================================================================================\n");
    struct newUser userData;
    if (fscanf(accountDetailsFile, "%d|%[^|]|%[^|]|%lld|%[^|]|%[^|]|%lld\n",
               &userData.acc_num, userData.name, userData.email, &userData.pn, userData.address, userData.pan_card, &userData.aadhar) == 7) {
        
        printf("| Name          | %-100s|\n", userData.name);
        printf("| Email         | %-100s|\n", userData.email);
        printf("| Phone Number  | %-100lld|\n", userData.pn);
        printf("| Address       | %-100s|\n", userData.address);
        printf("| PAN Card      | %-100s|\n", userData.pan_card);
        printf("| Aadhar Number | %-100lld|\n", userData.aadhar);
    } else {
        printf("|No account details found for Account Number %d|\n", bank_acc_num);
    }
    printf("=======================================================================================================================\n");

    fclose(accountDetailsFile);
}

void deleteAccount(int bank_acc_num) {
    char folderName[10];
    char fileName[50];

    FILE *userRecordFile = fopen("userrecord.txt", "r");
    if (userRecordFile == NULL) {
        printf("Error opening userrecord.txt for reading\n");
        exit(EXIT_FAILURE);
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file\n");
        fclose(userRecordFile);
        exit(EXIT_FAILURE);
    }

    int currentAccNum;
    while (fscanf(userRecordFile, "%d", &currentAccNum) == 1) {
        if (currentAccNum == bank_acc_num) {
            fscanf(userRecordFile, "%*[^\n]\n");
        } else {
            fprintf(tempFile, "%d", currentAccNum);
            int c;
            while ((c = fgetc(userRecordFile)) != EOF && c != '\n') {
                fputc(c, tempFile);
            }
            fputc('\n', tempFile);
        }
    }

    fclose(userRecordFile);
    fclose(tempFile);

    remove("userrecord.txt");
    rename("temp.txt", "userrecord.txt");

    sprintf(folderName, "%d", bank_acc_num);

    sprintf(fileName, "%s/account_details.txt", folderName);
    remove(fileName);

    sprintf(fileName, "%s/user_statement.txt", folderName);
    remove(fileName);

    sprintf(fileName, "%s/payees.txt", folderName);
    remove(fileName);

    if (_rmdir(folderName) != 0) {
        perror("Error deleting user folder");
    } else {
        printf("Account with Account Number %d deleted successfully.\n", bank_acc_num);
    }
}

void withdrawMoney(int bank_acc_num) {
    int amount;
    printf("Enter the amount to withdraw: ");
    scanf("%d", &amount);

    if (amount <= 0) {
        printf("Invalid withdrawal amount. Please enter a positive value.\n");
        return;
    }

    if (balanceCheck(bank_acc_num, amount)) {
        printf("Insufficient balance. Withdrawal canceled.\n");
        return;
    }

    addEntry(bank_acc_num, amount, "Withdrawal",1);
    printf("Withdrawal successful. Rs. %d deducted from the account.\n", amount);
}

void addEntry(int bank_acc_num, int amount, char remarks[], int type) {
    char fileName[50];
    sprintf(fileName, "%d/user_statement.txt", bank_acc_num);

    FILE *statementFile = fopen(fileName, "r+");
    if (statementFile == NULL) {
        printf("Error opening %s for reading and writing\n", fileName);
        exit(EXIT_FAILURE);
    }

    int currentBalance;
    fscanf(statementFile, "%d", &currentBalance);

    fseek(statementFile, 0, SEEK_END);

    time_t rawtime;
    struct tm *info;
    char date[20];
    time(&rawtime);
    info = localtime(&rawtime);
    strftime(date, sizeof(date), "%Y-%m-%d", info);

    if (type == 1) {
        fprintf(statementFile, "%s|%d|Dr|%s\n", date, amount, remarks);
    } else if (type == 2) {
        fprintf(statementFile, "%s|%d|Cr|%s\n", date, amount, remarks);
    }
    fseek(statementFile, 0, SEEK_SET);
    fprintf(statementFile, "%d", (type == 1) ? (currentBalance - amount) : (currentBalance + amount));

    fclose(statementFile);
}

void fundTransfer(int bank_acc_num) {
    if(!viewPayees(bank_acc_num)){
        printf("Please add a payee to transfer Funds.\n");
        return;
    }else{

        int payee_acc_num;
        printf("Enter the Bank Account Number of the Payee: ");
        scanf("%d", &payee_acc_num);

        if (!payeeCheck(payee_acc_num)) {
            printf("Payee with Bank Account Number %d not found.\n", payee_acc_num);
            return;
        }

        int amount;
        printf("Enter the amount to transfer: ");
        scanf("%d", &amount);

        if (amount <= 0) {
            printf("Invalid transfer amount. Please enter a positive value.\n");
            return;
        }

        if (balanceCheck(bank_acc_num, amount)) {
            printf("Insufficient balance. Fund transfer canceled.\n");
            return;
        }

        char senderRemarks[50];
        sprintf(senderRemarks, "Transferred to Bank Account %d", payee_acc_num);
        addEntry(bank_acc_num, amount, senderRemarks, 1);

        char payeeRemarks[50];
        sprintf(payeeRemarks, "Received from Bank Account %d", bank_acc_num);
        addEntry(payee_acc_num, amount, payeeRemarks, 2);

        printf("Fund transfer successful. Rs. %d transferred to Bank Account %d.\n", amount, payee_acc_num);
    }
}
void depositMoney(int bank_acc_num) {
    int amount;
    printf("Enter the amount to deposit: ");
    scanf("%d", &amount);

    if (amount <= 0) {
        printf("Invalid deposit amount. Please enter a positive value.\n");
        return;
    }

    addEntry(bank_acc_num, amount, "Deposit", 2);  // Type: Cr
    printf("Deposit successful. Rs. %d deposited into the account.\n", amount);
}
