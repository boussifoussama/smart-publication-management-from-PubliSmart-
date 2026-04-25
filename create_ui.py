#!/usr/bin/env python3
# Script pour générer le fichier smartmarket.ui complet avec les styles EXACTS de l'original

# Ce script crée un fichier UI qui combine:
# - Page 0: Login (avec EXACTEMENT le style original)
# - Page 1: Publication (avec EXACTEMENT le style original)

print("🎨 Génération du fichier smartmarket.ui avec vos styles EXACTS...")

with open('smartmarket.ui', 'w', encoding='utf-8') as f:
    f.write('''<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>SmartMarket</class>
 <widget class="QMainWindow" name="SmartMarket">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>1298</width>
    <height>768</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>SmartMarket</string>
  </property>
  <property name="styleSheet">
   <string notr="true">/* ===== THÈME GLOBAL – BLEU MARINE PREMIUM ===== */
* { font-family: "Inter", "Segoe UI", "Roboto", "Helvetica Neue", Arial, sans-serif; font-size: 14px; color: #E5E7EB; }
QMainWindow { background-color: #0B1F33; }
QWidget { background-color: transparent; }
QGroupBox { background-color: #0F2A44; border: 1px solid #1E3A8A; border-radius: 14px; margin-top: 22px; padding: 18px; }
QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 18px; font-size: 15px; font-weight: 800; color: #FFFFFF; }
QLabel { color: #E5E7EB; font-weight: 500; }
QTabWidget::pane { background-color: #0F2A44; border: 1px solid #1E3A8A; border-radius: 14px; margin-top: 10px; }
QTabBar::tab { background-color: #0B1F33; color: #CBD5E1; padding: 14px 32px; margin-right: 10px; border-radius: 12px; font-weight: 600; }
QTabBar::tab:selected { background-color: #1E40AF; color: #FFFFFF; font-weight: 900; }
QTabBar::tab:hover:!selected { background-color: #163B63; color: #FFFFFF; }
QLineEdit, QComboBox, QDateEdit, QSpinBox, QDoubleSpinBox { background-color: #0B1F33; border: 1px solid #1E3A8A; border-radius: 10px; padding: 12px; color: #FFFFFF; font-weight: 500; }
QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus { border-color: #3B82F6; background-color: #0F2A44; }
QPushButton { background-color: #1E40AF; color: #FFFFFF; border-radius: 12px; padding: 14px 30px; font-weight: 800; letter-spacing: 0.4px; }
QPushButton:hover { background-color: #2563EB; }
QPushButton:pressed { background-color: #1D4ED8; }
QPushButton:disabled { background-color: #475569; color: #CBD5E1; }
QTableWidget, QTableView { background-color: #0F2A44; border: 1px solid #1E3A8A; border-radius: 14px; gridline-color: #1E3A8A; alternate-background-color: #0B1F33; selection-background-color: #1E40AF; }
QTableWidget::item, QTableView::item { padding: 14px; color: #E5E7EB; font-weight: 500; }
QHeaderView::section { background-color: #0B1F33; color: #FFFFFF; padding: 14px; font-weight: 900; border: none; }
QCheckBox, QRadioButton { color: #E5E7EB; font-weight: 600; }
QCheckBox::indicator, QRadioButton::indicator { width: 16px; height: 16px; }
QProgressBar { background-color: #0B1F33; border: 1px solid #1E3A8A; border-radius: 12px; text-align: center; font-weight: 800; color: #FFFFFF; }
QProgressBar::chunk { background-color: #2563EB; border-radius: 10px; }
QMenuBar { background-color: #071526; color: #FFFFFF; font-weight: 800; }
QMenuBar::item:selected { background-color: rgba(255, 255, 255, 0.15); }
QStatusBar { background-color: #071526; color: #CBD5E1; }
#groupBox_11 { background-color: #071526; border: none; }
#groupBox_11 QPushButton { background-color: transparent; color: #E5E7EB; padding: 18px 24px; border-radius: 14px; font-size: 17px; font-weight: 700; }
#groupBox_11 QPushButton:hover { background-color: rgba(255,255,255,0.15); }
#pushButton_13 { background-color: rgba(255,255,255,0.25); font-weight: 900; }
</string>
  </property>
  <widget class="QWidget" name="centralwidget">
   <widget class="QStackedWidget" name="stackedWidgetMain">
    <property name="geometry">
     <rect>
      <x>0</x>
      <y>0</y>
      <width>1298</width>
      <height>730</height>
     </rect>
    </property>
    <property name="currentIndex">
     <number>0</number>
    </property>
    
    <!-- ==================== PAGE 0: LOGIN (STYLE ORIGINAL EXACT) ==================== -->
    <widget class="QWidget" name="pageLogin">''')
    
    # PAGE LOGIN avec le code EXACT de l'original
    f.write('''
     <widget class="QGroupBox" name="groupBox_11_login">
      <property name="geometry">
       <rect>
        <x>0</x>
        <y>-40</y>
        <width>661</width>
        <height>861</height>
       </rect>
      </property>
      <property name="title">
       <string>Official desktop app</string>
      </property>
      
      <widget class="QPushButton" name="pushButton_13">
       <property name="geometry">
        <rect>
         <x>220</x>
         <y>580</y>
         <width>161</width>
         <height>51</height>
        </rect>
       </property>
       <property name="text">
        <string>Envoyer</string>
       </property>
      </widget>
      
      <widget class="QPushButton" name="pushButton_15">
       <property name="geometry">
        <rect>
         <x>230</x>
         <y>370</y>
         <width>161</width>
         <height>51</height>
        </rect>
       </property>
       <property name="text">
        <string>entrer</string>
       </property>
      </widget>
      
      <widget class="QLabel" name="label_29">
       <property name="geometry">
        <rect>
         <x>70</x>
         <y>-50</y>
         <width>481</width>
         <height>361</height>
        </rect>
       </property>
       <property name="styleSheet">
        <string notr="true">image: url(:/new/prefix1/imagetouse.png-removebg-preview.png);</string>
       </property>
       <property name="text">
        <string/>
       </property>
      </widget>
      
      <widget class="QLineEdit" name="lineEdit_8">
       <property name="geometry">
        <rect>
         <x>300</x>
         <y>240</y>
         <width>291</width>
         <height>26</height>
        </rect>
       </property>
       <property name="styleSheet">
        <string notr="true">QLineEdit { color: black; background-color: white; border: 2px solid #555; border-radius: 5px; padding: 5px; font-size: 14px; }</string>
       </property>
       <property name="text">
        <string/>
       </property>
       <property name="dragEnabled">
        <bool>false</bool>
       </property>
      </widget>
      
      <widget class="QLineEdit" name="lineEdit_10">
       <property name="geometry">
        <rect>
         <x>300</x>
         <y>300</y>
         <width>291</width>
         <height>26</height>
        </rect>
       </property>
       <property name="styleSheet">
        <string notr="true">QLineEdit { color: black; background-color: white; border: 2px solid #555; border-radius: 5px; padding: 5px; font-size: 14px; }</string>
       </property>
      </widget>
      
      <widget class="QLineEdit" name="lineEdit_11">
       <property name="geometry">
        <rect>
         <x>300</x>
         <y>510</y>
         <width>291</width>
         <height>26</height>
        </rect>
       </property>
       <property name="styleSheet">
        <string notr="true">QLineEdit { color: black; background-color: white; border: 2px solid #555; border-radius: 5px; padding: 5px; font-size: 14px; }</string>
       </property>
      </widget>
      
      <widget class="QLabel" name="label">
       <property name="geometry">
        <rect>
         <x>170</x>
         <y>240</y>
         <width>111</width>
         <height>20</height>
        </rect>
       </property>
       <property name="text">
        <string>l'adresse mail :</string>
       </property>
      </widget>
      
      <widget class="QLabel" name="label_2">
       <property name="geometry">
        <rect>
         <x>170</x>
         <y>300</y>
         <width>111</width>
         <height>20</height>
        </rect>
       </property>
       <property name="text">
        <string>le mot de passe :</string>
       </property>
      </widget>
      
      <widget class="QLabel" name="label_3">
       <property name="geometry">
        <rect>
         <x>50</x>
         <y>210</y>
         <width>141</width>
         <height>20</height>
        </rect>
       </property>
       <property name="styleSheet">
        <string notr="true">font: 900 9pt "Segoe UI";</string>
       </property>
       <property name="text">
        <string>se connecter :</string>
       </property>
      </widget>
      
      <widget class="QLabel" name="label_4">
       <property name="geometry">
        <rect>
         <x>50</x>
         <y>450</y>
         <width>511</width>
         <height>21</height>
        </rect>
       </property>
       <property name="styleSheet">
        <string notr="true">font: 900 9pt "Segoe UI";</string>
       </property>
       <property name="text">
        <string>mot de passe oubliée ? pas de probleme entrez votre adresse mail : </string>
       </property>
      </widget>
     </widget>
     
     <widget class="QGroupBox" name="groupBox">
      <property name="geometry">
       <rect>
        <x>640</x>
        <y>-100</y>
        <width>771</width>
        <height>1241</height>
       </rect>
      </property>
      <property name="styleSheet">
       <string notr="true">background-color: rgb(255, 255, 255);</string>
      </property>
      <property name="title">
       <string>GroupBox</string>
      </property>
      <widget class="QLabel" name="label_5">
       <property name="geometry">
        <rect>
         <x>-50</x>
         <y>80</y>
         <width>731</width>
         <height>741</height>
        </rect>
       </property>
       <property name="styleSheet">
        <string notr="true">image: url(:/new/prefix1/Downloads/menuimage.png);</string>
       </property>
       <property name="text">
        <string>TextLabel</string>
       </property>
      </widget>
     </widget>
    </widget>
''')

    print("✅ Page Login créée avec style EXACT")
    
    # Fermer le fichier pour l'instant
    f.write('''
   </widget>
  </widget>
  <widget class="QMenuBar" name="menubar">
   <property name="geometry">
    <rect>
     <x>0</x>
     <y>0</y>
     <width>1298</width>
     <height>25</height>
    </rect>
   </property>
  </widget>
  <widget class="QStatusBar" name="statusbar"/>
 </widget>
 <resources/>
 <connections/>
</ui>''')

print("✅ Fichier UI créé (Page Login)")
print("\nℹ️  La page Publication sera ajoutée séparément...")
