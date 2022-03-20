DROP TABLE IF EXISTS [Transactions];
DROP TABLE IF EXISTS [Accounts];
DROP TABLE IF EXISTS [Customers];
DROP TABLE IF EXISTS [Credentials];

CREATE TABLE [Transactions]
(
    [TransactionId] INTEGER PRIMARY KEY NOT NULL,
    [FromAccount] INTEGER NOT NULL,
    [ToAccount] INTEGER NOT NULL,
    [Description] TEXT,
    [Amount] INTEGER NOT NULL,
    FOREIGN KEY ([FromAccount]) REFERENCES [Accounts] ([AccountId])
        ON DELETE NO ACTION ON UPDATE NO ACTION,
    FOREIGN KEY ([ToAccount]) REFERENCES [Accounts] ([AccountId])
        ON DELETE NO ACTION ON UPDATE NO ACTION
);

CREATE TABLE [Accounts]
(
    [AccountId] INTEGER PRIMARY KEY NOT NULL,
    [CustomerId] INTEGER NOT NULL,
    [Balance] INTEGER NOT NULL,
    [AccountType] TEXT NOT NULL,
    [OpenDate] TEXT NOT NULL,
    FOREIGN KEY ([CustomerId]) REFERENCES [Customers] ([CustomerId]) 
		ON DELETE NO ACTION ON UPDATE NO ACTION
);

CREATE TABLE [Customers]
(
    [CustomerId] INTEGER PRIMARY KEY NOT NULL,
    [CitizenNumber] INTEGER NOT NULL,
    [Name] TEXT NOT NULL,
    [Surname] TEXT NOT NULL,
    [Email] TEXT NOT NULL UNIQUE,
    [Mobile] TEXT NOT NULL UNIQUE CHECK (length(Mobile) >= 10),
    [Address] TEXT NOT NULL
);

CREATE TABLE [Credentials]
(
    [CredentialId] INTEGER PRIMARY KEY NOT NULL,
    [CustomerId] INTEGER NOT NULL,
    [Username] TEXT NOT NULL,
    [Password] TEXT NOT NULL,
    FOREIGN KEY ([CustomerId]) REFERENCES [Customers] ([CustomerId]) 
		ON DELETE NO ACTION ON UPDATE NO ACTION 
);

INSERT INTO [Customers] ([CitizenNumber], [Name], [Surname], [Email], [Mobile], [Address]) VALUES (123123123, 'Ramazan', 'Cetin', 'ramazancetinnn@gmail.com', '+31633322211', '1111AA, Amsterdam');
INSERT INTO [Customers] ([CitizenNumber], [Name], [Surname], [Email], [Mobile], [Address]) VALUES (234234234, 'Selen', 'Gecgel', 'selengecgel@gmail.com', '+31645455521', '1111AA, Amsterdam');
INSERT INTO [Customers] ([CitizenNumber], [Name], [Surname], [Email], [Mobile], [Address]) VALUES (345345345, 'Ahmet', 'Burali', 'ahmetburali@gmail.com', '+905314522211', 'kucukcekmece, Istanbul');

INSERT INTO [Accounts] ([CustomerId], [Balance], [AccountType], [OpenDate]) VALUES (1, 0, 'BASIC', datetime('now', 'localtime'));
INSERT INTO [Accounts] ([CustomerId], [Balance], [AccountType], [OpenDate]) VALUES (2, 0, 'BASIC', datetime('now', 'localtime'));
INSERT INTO [Accounts] ([CustomerId], [Balance], [AccountType], [OpenDate]) VALUES (3, 0, 'BASIC', datetime('now', 'localtime'));
INSERT INTO [Accounts] ([CustomerId], [Balance], [AccountType], [OpenDate]) VALUES (1, 0, 'INVESTMENT', datetime('now', 'localtime'));
