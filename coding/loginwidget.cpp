#include "loginwidget.h"
#include "ui_loginwidget.h"


loginWidget::loginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(u8"µÇÂ¼");
    QIcon icon(":/antrum/resouce/mainIcon.png");
    this->setWindowIcon(icon);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    
    this->setStyleSheet("loginWidget{background-image: url(:/antrum/resouce/background-imgae.png);}");

    ui->labelTitlie->setPixmap(QPixmap(":/antrum/resouce/text.png"));
    ui->labelTitlie->setAttribute(Qt::WA_TranslucentBackground);

    ui->accountWidget->setStyleSheet("QWidget{background-image: url(:/antrum/resouce/editRectangle.png)}");
    ui->accountLabel->setPixmap(QPixmap(":/antrum/resouce/accountEditLine.png"));
    ui->accountLineEdit->setStyleSheet("QLineEdit{background-color:transparent;color:white; border:none;}");


    ui->passwordWidget->setStyleSheet("QWidget{background-image: url(:/antrum/resouce/editRectangle.png)}");
    ui->passwordLabel->setPixmap(QPixmap(":/antrum/resouce/passwordEdit.png"));
    ui->passwordLineEdit->setStyleSheet("QLineEdit{background-color:transparent;color:white; border:none;}");
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    m_pwd = true;
   
    ui->eyeButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/eye.png);border:none;}QPushButton:hover {color: #555;}QPushButton:pressed{color:#333;}");


    ui->savePwdCheckBox->setStyleSheet("QCheckBox{ border-image: url();color:white; border:none;}");

    ui->errorLabel->setText(u8"ÕËºÅÃÜÂë´íÎó!");
    ui->errorLabel->setStyleSheet("QLabel{background-color:transparent;color:red;}");
    ui->errorLabel->hide();

    ui->loginButton->setStyleSheet("QPushButton{background-image: url(:/antrum/resouce/loginbtnRectangle.png);border:none;color:white;}QPushButton:hover {color: #555;}QPushButton:pressed{color:#333;}");



    QString account;
    QString pwd;
    m_logic.readPwd(account, pwd);

    if (account != "" && pwd != "")
    {
        ui->accountLineEdit->setText(account);
        ui->passwordLineEdit->setText(pwd);
    }

    ui->loginButton->setDefault(true);

    //ÐÅºÅ²Û
   QObject::connect(ui->eyeButton, SIGNAL(clicked()),this, SLOT(OnEyePassword()));
   QObject::connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(OnLoginBtn()));

   connect(ui->passwordLineEdit, &QLineEdit::textChanged, this, [=]() {
       ui->errorLabel->hide();
       });
}

loginWidget::~loginWidget()
{
    delete ui;
}

void loginWidget::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void loginWidget::OnLoginBtn()
{
   
    auto account = ui->accountLineEdit->text();
    auto pwd = ui->passwordLineEdit->text();
    if (m_logic.login(ui->savePwdCheckBox->isChecked(),account,pwd))
    {
        //emit JumpMainWidget();
        UltrasoundArtificialIntelligence* uai = new UltrasoundArtificialIntelligence();
        uai->show();
        this->hide();

    }
    else
    {
        ui->errorLabel->show();
    }
}


void loginWidget::OnEyePassword()
{
    if (m_pwd)
    {
        ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
        m_pwd = false;
    }
    else
    {
        ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
        m_pwd = true;
    }
}


void loginWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        exit(0);
    }
}
