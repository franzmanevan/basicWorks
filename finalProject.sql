/*

Group Members: Evan Franzman
Database Description: This database is designed to emulate a banking database
to store the information and relationships between customers, accounts, bankers,
corresponding debit and credit cards, and all concurrent charges on those cards.

*/

--============================================================= CREATE TABLES
USE master
GO

IF DB_ID('FinalProject') IS NOT NULL DROP DATABASE FinalProject
GO

CREATE DATABASE FinalProject
GO

USE FinalProject
GO

CREATE TABLE Banker (
    BankerId        INT         NOT NULL        PRIMARY KEY         IDENTITY,
    firstName       VARCHAR(20) NOT NULL,
    lastName        VARCHAR(20) NOT NULL,
    email           VARCHAR(50) NOT NULL
);
GO

CREATE TABLE AccountTypes (
    accountTypeId       INT         NOT NULL        PRIMARY KEY         IDENTITY,
    description         VARCHAR(50) NOT NULL
);
GO

CREATE TABLE Account (
    accountId       INT         NOT NULL        PRIMARY KEY         IDENTITY,
    accountTypeId   INT         NOT NULL        FOREIGN KEY         REFERENCES AccountTypes(accountTypeId),
    balance         FLOAT       NOT NULL        DEFAULT(0.0),
    interestRate    FLOAT       NOT NULL        DEFAULT(0.0)
);
GO

CREATE TABLE DebitCard (
    debitCardId         INT         NOT NULL        PRIMARY KEY         IDENTITY,
    accountId           INT         NOT NULL        FOREIGN KEY         REFERENCES Account(accountId),
    cardNumber          INT         NOT NULL
);
GO

CREATE TABLE Transactions (
    logId           INT         NOT NULL        PRIMARY KEY         IDENTITY,
    accountId       INT         NOT NULL        FOREIGN KEY         REFERENCES Account(accountId),
    [transaction]   FLOAT       NOT NULL,
    transactionDate DATE        NOT NULL
);
GO

CREATE TABLE Withdrawls (
    withdrawlId         INT         NOT NULL        PRIMARY KEY         IDENTITY,
    amountWithdrawn     FLOAT       NOT NULL,
    [date]              DATE        NOT NULL,
    logId               INT         NOT NULL        FOREIGN KEY         REFERENCES Transactions(logId),
    debitCardId         INT         NOT NULL        FOREIGN KEY         REFERENCES DebitCard(debitCardId)
);
GO

CREATE TABLE Deposits (
    depositId       INT         NOT NULL        PRIMARY KEY         IDENTITY,
    amountDeposited FLOAT       NOT NULL, 
    [date]          DATE        NOT NULL,
    logId           INT         NOT NULL        FOREIGN KEY         REFERENCES Transactions(logId)
);
GO

CREATE TABLE AccountHolder (
    accountHolderId         INT         NOT NULL        PRIMARY KEY         IDENTITY,
    bankerId                INT         NOT NULL        FOREIGN KEY         REFERENCES Banker(bankerId),
    accountId               INT         NOT NULL        FOREIGN KEY         REFERENCES Account(accountId),
    firstName               VARCHAR(20) NOT NULL,
    lastName                VARCHAR(20) NOT NULL,
    email                   VARCHAR(50) NOT NULL,
    phoneNumber             VARCHAR(20) NOT NULL
);
GO

CREATE TABLE CreditCard (
    creditCardId        INT         NOT NULL        PRIMARY KEY         IDENTITY, 
    accountHolderId     INT         NOT NULL        FOREIGN KEY         REFERENCES AccountHolder(accountHolderId),
    creditLimit         FLOAT       NOT NULL,
    currentBalance      FLOAT       NOT NULL,
    balanceDueDate      DATE        NOT NULL
);
GO

CREATE TABLE CreditCharge (
    creditChargeId      INT         NOT NULL        PRIMARY KEY         IDENTITY,
    creditCardId        INT         NOT NULL        FOREIGN KEY         REFERENCES CreditCard(creditCardId),
    amountCharged       FLOAT       NOT NULL,
    [date]              DATE        NOT NULL
);
GO

--============================================================= CREATE VIEWS
-- In order for my script to run without fail, these either needed to be commented out
-- or in a separate batch. To run, remove commented "--" and highlight only 
-- one create statement at a time. 
--
--CREATE VIEW Accountvw
--AS 
--SELECT ac.firstname, ac.lastName, c.creditCardId, d.cardNumber
--FROM AccountHolder ac, Account a, CreditCard c, DebitCard d
--WHERE ac.accountId = a.accountId             AND
--      c.accountHolderId = ac.accountHolderId AND
--      d.accountId = a.accountId
--
--CREATE VIEW BankerClientsvw
--AS
--SELECT b.firstName, b.lastName, COUNT(ac.accountHolderId) AS 'numberOfClients'
--FROM Banker b, AccountHolder ac
--WHERE b.BankerId = ac.bankerId
--GROUP BY b.firstName, b.lastName


--============================================================= CREATE STORED PROCEDURES
--CREATE PROCEDURE AddUpdateDeleteUser 
--    @determiner     INT, 
--    @accountHolderId INT,
--    @firstName      VARCHAR(20),
--    @lastName       VARCHAR(20),
--    @bankerId       INT, 
--    @accountId      INT, 
--    @email          VARCHAR(50),
--    @phoneNumber    VARCHAR(20)
--AS BEGIN
--    IF EXISTS (SELECT NULL FROM AccountHolder WHERE firstName = @firstName and lastName = @lastName) BEGIN
--        IF (@determiner = 1) BEGIN
--            UPDATE AccountHolder SET 
--                firstName       = @firstName,
--                lastName        = @lastName,
--                bankerId        = @bankerId,
--                accountId       = @accountId,
--                email           = @email,
--                phoneNumber     = @phoneNumber
--            WHERE accountHolderId = @accountHolderId
--        END ELSE IF (@determiner = 2) BEGIN
--            DELETE 
--            FROM AccountHolder 
--            WHERE accountHolderId = @accountHolderId
--        END ELSE BEGIN
--            INSERT INTO AccountHolder (bankerId, AccountId, firstName, lastName, email, phoneNumber) VALUES
--                (@bankerId, @accountId, @firstName, @lastName, @email, @phoneNumber)
--        END
--    END
--END
--
--CREATE PROCEDURE AddUpdateDeleteDebitCard
--    @determiner     INT,
--    @debitCardId    INT,
--    @accountId      INT,
--    @cardNumber     INT
--AS BEGIN
--        IF EXISTS (SELECT NULL FROM DebitCard WHERE debitCardId = @debitCardId AND accountId = @accountId) BEGIN
--            IF (@determiner = 1) BEGIN
--                UPDATE DebitCard SET 
--                    cardNumber = @cardNumber
--                WHERE debitCardId = @debitCardId AND accountId = @accountId
--            END ELSE IF (@determiner = 2) BEGIN
--                DELETE 
--                FROM DebitCard
--                WHERE debitCardId = @debitCardId AND accountId = @accountId
--            END
--        END ELSE BEGIN
--            INSERT INTO DebitCard (accountId, cardNumber) VALUES 
--                (@accountId, @cardNumber)
--        END
--END
--
--CREATE PROCEDURE AddUpdateDeleteCreditCard
--    @determiner           INT,
--    @creditCardId         INT,
--    @accountHolderId      INT,
--    @creditLimit          INT
--AS BEGIN
--    IF EXISTS (SELECT NULL FROM CreditCard WHERE creditCardId = @creditCardId AND accountHolderId = @accountHolderId) BEGIN
--            
--            IF (@determiner = 2) BEGIN
--                DELETE 
--                FROM CreditCard
--                WHERE creditCardId = @creditCardId AND accountHolderId = @accountHolderId
--            END
--        END ELSE BEGIN
--            INSERT INTO CreditCard (accountHolderId, creditLimit, currentBalance, balanceDueDate) VALUES 
--                (@accountHolderId, @creditLimit, 0, GETDATE())
--        END
--END
--
--CREATE PROCEDURE AddUpdateDeleteAccount
--    @determiner     INT,
--    @accountId      INT,
--    @accountTypeId  INT, 
--    @balance        FLOAT,
--    @interestRate   FLOAT
--AS BEGIN
--    IF EXISTS (SELECT NULL FROM Account WHERE accountId = @accountId) BEGIN
--        IF (@determiner = 1) BEGIN
--            UPDATE Account SET
--                balance = @balance, 
--                interestRate = @interestRate
--            WHERE accountId = @accountId
--        END ELSE IF (@determiner = 2) BEGIN 
--            DELETE 
--            FROM Account
--            WHERE accountId = @accountId
--        END
--    END ELSE BEGIN
--        INSERT INTO Account (accountTypeId, balance, interestRate) VALUES
--            (@accountTypeId, @balance, @interestRate)
--    END
--END
--
--CREATE PROCEDURE Withdraw 
--    @accountId      INT, 
--    @amount         FLOAT,
--    @logId          INT,
--    @debitCardId    INT
--AS BEGIN 
--    IF EXISTS (SELECT NULL FROM Account WHERE accountId = @accountId) BEGIN
--        UPDATE Account SET
--            balance = balance - @amount
--        WHERE accountId = @accountId
--        INSERT INTO Withdrawls (amountWithdrawn, [date], logId, debitCardId) VALUES
--            (0 - @amount, GETDATE(), @logId, @debitCardId)
--    END
--END
--
--CREATE PROCEDURE Deposit 
--    @amount     FLOAT,
--    @accountId  INT 
--AS BEGIN
--    IF EXISTS (SELECT NULL FROM Account WHERE accountId = @accountId) BEGIN
--        UPDATE Account SET 
--            balance = balance + @amount
--        WHERE accountId = @accountId
--        INSERT INTO Deposits (amountDeposited, [date], logId) VALUES
--            (@amount, GETDATE(), @accountId)
--    END
--END
--============================================================= CREATE TRIGGERS

--============================================================= CREATE FUNCTIONS

--============================================================= TABLE POPULATION
INSERT INTO Banker (firstName, lastName, email) VALUES
    ('Peter', 'Frampton', 'pframp@gmail.com'),
    ('Jim', 'Bob', 'jimbob@gmail.com'),
    ('Alice', 'Cooper', 'acoop@yahoo.com'),
    ('Henry', 'Deal', 'hdeal@miamioh.edu'),
    ('Micki', 'Johnson', 'mj@google.com')

INSERT INTO AccountTypes ([description]) VALUES 
    ('Savings Account'),
    ('Checkings Account'),
    ('Money Market Account'),
    ('CD')

INSERT INTO ACCOUNT (accountTypeId, balance, interestRate) VALUES
    (1, 100.00, .01),
    (2, 49.89, 0),
    (3, 45434.37, 6),
    (4, 1000, 4),
    (1, 0, .02)

INSERT INTO DebitCard (accountId, cardNumber) VALUES 
    (1, 123456789),
    (2, 987654321),
    (5, 192837465)

INSERT INTO Transactions (accountId, [transaction], transactionDate) VALUES
    (1, 50, '1/6/2005'),
    (1, -90, '2/7/2006'),
    (2, -40, '2/5/2006'), 
    (5, 50, '2/9/2006')

INSERT INTO Withdrawls (amountWithdrawn, [date], logId, debitCardId) VALUES
    (-90, '2/7/2006', 2, 1),
    (-40, '2/5/2006', 3, 2)

INSERT INTO Deposits (amountDeposited, [date], logId) VALUES
    (50, '1/6/2005', 1),
    (50, '2/9/2006', 4)

INSERT INTO AccountHolder (bankerId, AccountId, firstName, lastName, email, phoneNumber) VALUES
    (1, 1, 'Evan', 'Franzman', 'evanfranzman@tdu.me', '937-555-6113'),
    (2, 2, 'Kenny', 'Franzman', 'kfranzman@csu.edu', '937-555-5463'),
    (3, 3, 'Roy', 'Howard', 'roysyourboy@google.me', '937-555-1234'),
    (4, 4, 'Josh', 'Peck', 'jp@yahoo.com', '937-555-9876'),
    (5, 5, 'Ethan', 'Allen', 'ea@woh.rr.com', '937-555-5638')

INSERT INTO CreditCard (accountHolderId, creditLimit, currentBalance, balanceDueDate) VALUES
    (1, 1200, 1.50, '3/1/2006'),
    (2, 2500, 432.65, '3/10/2006')

INSERT INTO CreditCharge (creditCardId, amountCharged, [date]) VALUES
    (1, 1.50, '2/27/2006'),
    (2, 432.65, '2/14/2006')

--============================================================= TEST CALLS TO STORED PROCEDURES