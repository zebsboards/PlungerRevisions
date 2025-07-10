using System;
using System.Drawing;
using System.IO.Ports;
using System.Net;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace VirtuaPin_Configuration_Utility
{
    public partial class FormHome : Form
    {
        string SWrevision = "100-00-00-47";
        public FormHome()
        {
            InitializeComponent();
        }
        private void FormHome_Load(object sender, EventArgs e)
        {
            Thread t = new Thread(new ThreadStart(SplashStart));
            t.Start();
            Thread.Sleep(2000);
            string[] ports = SerialPort.GetPortNames();
            cmbPorts.Items.AddRange(ports);
            cmbBaudRate.SelectedItem = "2000000";
            this.Invoke(new EventHandler(FormHomeConnect));
            t.Abort();

        }
        private void FormHomeConnect(object sender, EventArgs e)
        {
            this.Invoke(new EventHandler(btnAutoConnect_Click));
            if (serialPort1.IsOpen)
            {
                cfgInfoBox.Visible = false;
                picBoxSettings.Visible = false;
                progressBar.Visible = false;
                label37.Visible = false;
                lblDownloadInf.Visible = false;
                label39.Visible = false;
                lblFwUp.Visible = false;
                lblFW2.Visible = false;
                label41.Visible = false;
                label42.Visible = false;
                label44.Visible = false;
                lblSwUp.Visible = false;
                lblSW2.Visible = false;
                label10.Text = SWrevision;
                label43.Text = SWrevision;
                btnIncrement.Text = "1";
                btnDzIncrement.Text = "1";
            }
            else
            {
                cfgInfoBox.Visible = false;
                picBoxSettings.Visible = false;
                progressBar.Visible = false;
                label37.Visible = false;
                lblDownloadInf.Visible = false;
                label39.Visible = false;
                lblFwUp.Visible = false;
                lblFW2.Visible = false;
                label41.Visible = false;
                label42.Visible = false;
                label44.Visible = false;
                lblSwUp.Visible = false;
                lblSW2.Visible = false;
                label10.Text = SWrevision;
                label43.Text = SWrevision;
                btnDisconnect.Enabled = false;
                picwarning.Visible = false;
                btnUpdateCfg.Enabled = false;
                btnFactoryReset.Enabled = false;
                btnReboot.Enabled = false;
                btnTestHW.Enabled = false;
                btnDzIncrement.Text = "1";
                btnIncrement.Text = "1";
                btnDzM.Enabled = false;
                btnDzP.Enabled = false;
                btnCal.Enabled = false;
                btnMinus.Enabled = false;
                btnPlus.Enabled = false;
                btnRefresh.Enabled = false;
                cmBoxButton.Text = "1";
                cmBoxKey.Text = "SHIFT_LEFT";
                cmBoxButtonUse.Text = "L Flipper";
            }
        }
        public void SplashStart()
        {
            Application.Run(new SplashScreen());
        }
        //
        //*********************************************************** Serial Connection Controls ************************************************
        //

        int PortFound = 0;
        
        private void btnAutoConnect_Click(object sender, EventArgs e)
        {
            VerifiedFlag = 0;
            StartupSettingsFlag = 0;
            PortFound = 0;
            foreach (string sp in SerialPort.GetPortNames())
            {
                try
                {
                    cmbPorts.Text = sp;
                    serialPort1.PortName = sp;
                    serialPort1.BaudRate = Convert.ToInt32(cmbBaudRate.Text);
                    serialPort1.DtrEnable = true; serialPort1.RtsEnable = true;
                    serialPort1.Open();
                    if (serialPort1.IsOpen)
                    {
                        serialPort1.WriteLine("V");
                        Task.Delay(100).Wait();
                        if (DataIn != null)
                        {
                            try
                            {
                                rtbDataIn.Text = DataIn;
                                string[] reply = DataIn.Split(',');
                                if (reply[0] == "VirtuaPin" && reply[1] == "20A0")
                                {
                                    rtbDataIn.Text = " Firmware Verified";
                                    rtbManufacturer.SelectionAlignment = HorizontalAlignment.Center;
                                    rtbManufacturer.Text = reply[0];
                                    rtbManufacturerD.SelectionAlignment = HorizontalAlignment.Center;
                                    rtbManufacturerD.Text = reply[0];
                                    rtbVID.SelectionAlignment = HorizontalAlignment.Center;
                                    rtbVID.Text = reply[1];
                                    rtbVIDD.SelectionAlignment = HorizontalAlignment.Center;
                                    rtbVIDD.Text = reply[1];
                                    rtbPID.SelectionAlignment = HorizontalAlignment.Center;
                                    rtbPID.Text = reply[2];
                                    rtbPIDD.SelectionAlignment = HorizontalAlignment.Center;
                                    rtbPIDD.Text = reply[2];
                                    rtbREV.SelectionAlignment = HorizontalAlignment.Center;
                                    rtbREV.Text = reply[3];
                                    rtbREVD.SelectionAlignment = HorizontalAlignment.Center;
                                    rtbREVD.Text = reply[3];
                                    serialPort1.DataReceived += new SerialDataReceivedEventHandler(SerialPort1_DataReceived);
                                    PortFound = 1;
                                }
                            }
                            catch (Exception err)
                            {
                                //MessageBox.Show(err.Message, "Communication Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                Task.Delay(1000).Wait();
                                serialPort1.Close();
                            }
                        }
                        else
                        {
                            serialPort1.Close();
                        }
                    }
                }
                catch (Exception err)
                {
                    if (serialPort1 != null)
                    {
                        serialPort1.Close();
                        Task.Delay(1000).Wait();
                        //rtbDataIn.Text = "PORT ERROR";
                        //MessageBox.Show(err.Message, "Communication Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
            if (PortFound == 1)
            {
               this.Invoke(new EventHandler(ConnectStatus));
            }
            else
            {
                rtbDataIn.Text = "Controller Not Found";
            }
        }
        private void btnRescan_Click(object sender, EventArgs e)
        {
            cmbPorts.Items.Clear();
            string[] ports = SerialPort.GetPortNames();
            cmbPorts.Items.AddRange(ports);
            cmbPorts.SelectedIndex = 0;
            PortFound = 0;
        }
        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.Close();
                this.Invoke(new EventHandler(DisconnectStatus));
            }
        }
        private void FormHome_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                btnDisconnect.Enabled = true;
                if (ButtonTest != 0)
                {
                    btnTestButtons.PerformClick();
                    Task.Delay(100).Wait();
                }
                serialPort1.Close();
                Task.Delay(200).Wait();
                System.Windows.Forms.Application.Exit();
            }
        }
        private void DisconnectStatus(object sender, EventArgs e)
        {
            lblStatus.ForeColor = Color.Gold;
            lblStatus.Text = "DISCONNECTED";
            lblStatus.BackColor = Color.Black;
            rtbDataIn.Text = "DISCONNECTED";
            btnDisconnect.Enabled = false;
            btnAutoConnect.Enabled = true;
            btnUpdateCfg.Enabled = false;
            btnFactoryReset.Enabled = false;
            btnReboot.Enabled = false;
            btnTestHW.Enabled = false;
            btnDzM.Enabled = false;
            btnDzP.Enabled = false;
            btnCal.Enabled = false;
            btnMinus.Enabled = false;
            btnPlus.Enabled = false;
            btnRefresh.Enabled = false;
            btnTestButtons.Enabled = false;
            picBoxSettings.Visible = false;
            cfgInfoBox.Visible = false;
            rtbDiagStatus.BackColor = Color.Black;
            rtbDiagStatus.ForeColor = Color.Gold;
            rtbDiagStatus.Text = "DISCONNECTED";
        }
        private void ConnectStatus(object sender, EventArgs e)
        {
            lblStatus.ForeColor = Color.Black;
            lblStatus.Text = "CONNECTED";
            lblStatus.BackColor = Color.LimeGreen;
            btnAutoConnect.Enabled = false;
            btnDisconnect.Enabled = true;
            btnUpdateCfg.Enabled = true;
            btnFactoryReset.Enabled = true;
            btnReboot.Enabled = true;
            btnTestHW.Enabled = true;
            btnDzM.Enabled = true;
            btnDzP.Enabled = true;
            btnCal.Enabled = true;
            btnMinus.Enabled = true;
            btnPlus.Enabled = true;
            btnRefresh.Enabled = true;
            btnTestButtons.Enabled = true;
            rtbDiagStatus.BackColor = Color.Green;
            rtbDiagStatus.ForeColor = Color.Black;
            rtbDiagStatus.Text = "CONNECTED";
            if (VerifiedFlag == 0)
            {
                VerifiedFlag = 1;
                lblCom.Text = cmbPorts.Text;
                lblBaud.Text = cmbBaudRate.Text;
                Count = 1;
                StartupSettingsFlag = 1;
                serialPort1.Write("E");
                //this.Invoke(new EventHandler(StartUpSettings));
            }
        }
        //
        //******************************** Firmware Verification, Startup Settings & Serial Data Recieved ***************************************
        //

        int VerifiedFlag = 0;

        private void VerifyFirmware()
        {
            rtbDataIn.Text = " Verifying Firmware";
            Task.Delay(100).Wait();
            try
            {
                String[] FirmwareRev = DataIn.Split(',');
                String Manuf = FirmwareRev[0];
                String VID = FirmwareRev[1];
                String PID = FirmwareRev[2];
                if (Manuf != "VirtuaPin")
                {
                    rtbManufacturer.Text = "ERROR!";
                    rtbDataIn.Text = "Not Verified Not a VirtuaPin Product";
                    VerifiedFlag = 0;
                }
                if (VID != "20A0")
                {
                    rtbVID.Text = "ERROR!";
                    rtbDataIn.Text = "Not Verified VID Mismatch";
                    VerifiedFlag = 0;
                }
                if (PID != "41B9")
                {
                    rtbPID.Text = "ERROR!";
                    rtbDataIn.Text = "Not Verified PID Mismatch";
                    VerifiedFlag = 0;
                }
                else
                {
                    rtbDataIn.Text = " Firmware Verified";
                    rtbManufacturer.SelectionAlignment = HorizontalAlignment.Center;
                    rtbManufacturer.Text = Manuf;
                    rtbManufacturerD.SelectionAlignment = HorizontalAlignment.Center;
                    rtbManufacturerD.Text = Manuf;
                    rtbVID.SelectionAlignment = HorizontalAlignment.Center;
                    rtbVID.Text = VID;
                    rtbVIDD.SelectionAlignment = HorizontalAlignment.Center;
                    rtbVIDD.Text = VID;
                    rtbPID.SelectionAlignment = HorizontalAlignment.Center;
                    rtbPID.Text = PID;
                    rtbPIDD.SelectionAlignment = HorizontalAlignment.Center;
                    rtbPIDD.Text = PID;
                    rtbREV.SelectionAlignment = HorizontalAlignment.Center;
                    rtbREV.Text = FirmwareRev[3];
                    rtbREVD.SelectionAlignment = HorizontalAlignment.Center;
                    rtbREVD.Text = FirmwareRev[3];
                    this.Invoke(new EventHandler(ConnectStatus));
                }
            }
            catch (Exception error)
            {
                MessageBox.Show(error.Message);
            }
            
        }

        string DataIn;

        private void SerialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            if (VerifiedFlag != 1)
            {
                Task.Delay(1).Wait();
                DataIn = serialPort1.ReadLine();
            }
            else
            {
                if (StartupSettingsFlag == 1)
                {
                    Task.Delay(1).Wait();
                    DataIn = serialPort1.ReadLine();
                    this.Invoke(new EventHandler(StartUpSettings));
                }
                else if (ButtonTest == 1)
                {
                    DataIn = serialPort1.ReadLine();
                    ButtonTestRoutine();
                }
                else if (plungerCalFlag == 1 || reportFlag == 1 || updateFlag == 1)
                {
                    
                }
                else
                {
                    DataIn = serialPort1.ReadLine();
                }
            }
        }

        int Count = 0;
        int StartupSettingsFlag;   /// load in the read settings at startup loop
        Byte[] ConfigSet = new Byte[32];
        Byte[] PrevConfigSet = new byte[32];
        Byte[] ConfigKeys = new Byte[32];
        Byte[] LabelCodesReceived = new byte[32];
        Int16[] ConfigAnalog = new Int16[32];
        Int16[] PrevConfigAnalog = new Int16[32];

        private void StartUpSettings(object sender, EventArgs e)
        {
            rtbDataIn.Text = "Retrieving Current Settings... Stand By";
            rtbDataIn2.Text = "Retrieving Current Settings... Stand By";
            try
            {
                DataIn = DataIn.Trim();
                char[] LeaderRemoved = { 'E', 'F', 'G', 'H' };
                DataIn = DataIn.Trim(LeaderRemoved);
                string[] OutData = DataIn.Split(',');
                if (Count == 1)
                {
                    for (int a = 0; a < 25; a++)
                    {
                        ConfigSet[a] = byte.Parse(OutData[a]);
                        PrevConfigSet[a] = ConfigSet[a];
                    }
                    if (ConfigSet[0] == 0)                                          // Startup Controller Mode Select
                    {
                        comboBoxGPKBstartMode.Text = "Keyboard";
                    }
                    else
                    {
                        comboBoxGPKBstartMode.Text = "Gamepad";
                    }
                    if (ConfigSet[1] == 1)                                          // Indicator Mode
                    {
                        comboBoxIndMode.Text = "ON at Rest";
                    }
                    else if (ConfigSet[1] == 2)
                    {
                        comboBoxIndMode.Text = "OFF at Rest";
                    }
                    else if (ConfigSet[1] == 3)
                    {
                        comboBoxIndMode.Text = "ON at Rest Inverted";
                    }
                    else if (ConfigSet[1] == 4)
                    {
                        comboBoxIndMode.Text = "OFF at Rest Inverted";
                    }
                    else
                    {
                        comboBoxIndMode.Text = "Disabled";
                    }
                    if (ConfigSet[3] == 0)                                          // Board Orientation
                    {
                        comboBoxOrientation.Text = "Vertical Left Panel";
                    }
                    else if (ConfigSet[3] == 1)
                    {
                        comboBoxOrientation.Text = "Vertical Front Panel";
                    }
                    else if (ConfigSet[3] == 2)
                    {
                        comboBoxOrientation.Text = "Vertical Back Panel";
                    }
                    else if (ConfigSet[3] == 3)
                    {
                        comboBoxOrientation.Text = "Vertical Right Panel";
                    }
                    else if (ConfigSet[3] == 4)
                    {
                        comboBoxOrientation.Text = "Horizontal Cable Left";
                    }
                    else if (ConfigSet[3] == 5)
                    {
                        comboBoxOrientation.Text = "Horizontal Cable Front";
                    }
                    else if (ConfigSet[3] == 6)
                    {
                        comboBoxOrientation.Text = "Horizontal Cable Right";
                    }
                    else if (ConfigSet[3] == 7)
                    {
                        comboBoxOrientation.Text = "Horizontal Cable Back";
                    }
                    else
                    {
                        comboBoxOrientation.Text = "Invalid Entry";
                    }
                    if (ConfigSet[6] == 0)                                          // Tilt Selection
                    {
                        comboBoxTiltEnable.Text = "Disabled";
                        rtBoxTiltValX.SelectionAlignment = HorizontalAlignment.Center;
                        rtBoxTiltValX.Text = "Disabled";
                        rtBoxTiltValY.SelectionAlignment = HorizontalAlignment.Center;
                        rtBoxTiltValY.Text = "Disabled";
                        rtBoxTbtn.SelectionAlignment = HorizontalAlignment.Center;
                        rtBoxTbtn.Text = "Disabled";
                        radioButton4.Enabled = false;
                        radioButton5.Enabled = false;
                        radioButton6.Enabled = false;
                    }
                    else
                    {
                        comboBoxTiltEnable.Text = "Enabled";
                    }
                    rtbLBPos.Text = ConfigSet[7].ToString();                        // Launch Ball Button Position (adjusted actual use value)
                    if (ConfigSet[8] == 0)                                          // Hat Switch Selection
                    {
                        comboBoxHatSw.Text = "Disabled";
                    }
                    else
                    {
                        comboBoxHatSw.Text = "Enabled";
                    }
                    rtbCDval.Text = ConfigSet[9].ToString();                        // Coin Door Keypress Assignment
                    rtbLBval.Text = ConfigSet[10].ToString();                       // Launch Ball Keypress Assignment
                    if (ConfigSet[11] == 1)                                         // Sensor Selection
                    {
                        comboBoxSensor.Text = "IR Sensor";
                    }
                    else if (ConfigSet[11] == 2)
                    {
                        comboBoxSensor.Text = "Potentiometer";
                    }
                    else
                    {
                        comboBoxSensor.Text = "Disabled";
                    }
                    if (OutData[13] == "0")
                    {
                        comboBoxAccSensitivity.Text = "Disabled";
                    }
                    else
                    {
                        comboBoxAccSensitivity.Text = OutData[13] + "g";            // Accelerometer Sensitivity
                    }
                    comboBoxBrightness.Text = OutData[14];                          // Indicator Brightness
                    if (ConfigSet[15] == 0)                                         // TrackBall Enable
                    {
                        cmBoxTrackB.Text = "Coming Soon";
                        cmBoxTrackB.Enabled = false;
                    }
                    else
                    {
                        cmBoxTrackB.Text = "Coming Soon";
                        cmBoxTrackB.Enabled = false;
                    }
                    rtbCDpos.Text = ConfigSet[16].ToString();                        // Coin Door Button Assignment (adjusted actual use value)
                    rtBoxDeadzone.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxDeadzone.Text = OutData[2];                                 // Plunger Deadzone
                    rtBoxXgain.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxXgain.Text = OutData[4];
                    rtBoxYgain.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxYgain.Text = OutData[5];
                    if (ConfigSet[6] != 0)
                    {
                        rtBoxTbtn.SelectionAlignment = HorizontalAlignment.Center;
                        rtBoxTbtn.Text = OutData[12];
                    }
                    else
                    {
                        rtBoxTbtn.SelectionAlignment = HorizontalAlignment.Center;
                        rtBoxTbtn.Text = "Disabled";
                    }
                    //Task.Delay(500).Wait();
                    serialPort1.Write("A");
                    Count = Count + 1;
                }

                // 2nd String of Data
                else if (Count == 2)
                {
                    rtbDataIn.Text = "Retrieving Keys Assignments...Stand By";
                    rtbDataIn2.Text = "Retrieving Keys Assignments...Stand By";
                    Task.Delay(100).Wait();
                    for (int a = 0; a < 32; a++)
                    {
                        ConfigKeys[a] = Byte.Parse(OutData[a]);
                    }
                    string hex = BitConverter.ToString(ConfigKeys);
                    rtbKeyCommands.Text = hex;
                    //Task.Delay(500).Wait();
                    serialPort1.Write("A");
                    Count = Count + 1;
                }
                // 3rd String of Data
                else if (Count == 3)
                {
                    rtbDataIn.Text = "Retrieving Label Code Assignments...Stand By";
                    rtbDataIn2.Text = "Retrieving Label Code Assignments...Stand By";
                    Task.Delay(100).Wait();
                    for (int a = 0; a < 32; a++)
                    {
                        LabelCodesReceived[a] = Byte.Parse(OutData[a]);
                    }
                    string hex = BitConverter.ToString(LabelCodesReceived);
                    rtbKeyLabels.Text = hex;
                    //Task.Delay(500).Wait();
                    serialPort1.Write("A");
                    Count = Count + 1;
                }
                // 4th String of Data
                else if (Count == 4)
                {
                    rtbDataIn.Text = "Retrieving Analog Settings...Stand By";
                    rtbDataIn2.Text = "Retrieving Analog Settings...Stand By";
                    Task.Delay(100).Wait();
                    for (int a = 0; a < 6; a++)
                    {
                        ConfigAnalog[a] = Int16.Parse(OutData[a]);
                        PrevConfigAnalog[a] = ConfigAnalog[a];
                    }
                    rtBoxDraw.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxDraw.Text = OutData[0];
                    rtBoxPush.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxPush.Text = OutData[1];
                    rtbXdZone.SelectionAlignment = HorizontalAlignment.Center;
                    rtbXdZone.Text = OutData[2];
                    rtbYdZone.SelectionAlignment = HorizontalAlignment.Center;
                    rtbYdZone.Text = OutData[3];
                    if (ConfigSet[6] != 0)
                    {
                        rtBoxTiltValX.SelectionAlignment = HorizontalAlignment.Center;
                        rtBoxTiltValX.Text = OutData[4];
                        rtBoxTiltValY.SelectionAlignment = HorizontalAlignment.Center;
                        rtBoxTiltValY.Text = OutData[5];
                    }
                    else
                    {
                        rtBoxTiltValX.SelectionAlignment = HorizontalAlignment.Center;
                        rtBoxTiltValX.Text = "Disabled";
                        rtBoxTiltValY.SelectionAlignment = HorizontalAlignment.Center;
                        rtBoxTiltValY.Text = "Disabled";
                    }
                    SortKeyEntries();
                    rtbDataIn.SelectionAlignment = HorizontalAlignment.Center;
                    rtbDataIn.Text = "Settings Retrieval Complete";
                    rtbDataIn2.Text = "Settings Retrieval Complete";
                    //Task.Delay(500).Wait();
                    lblpicWarning2.Visible = false;
                    picwarning.Visible = false;
                    StartupSettingsFlag = 0;
                    Count = 0;
                    rtbDataIn.Text = "Ready";
                    rtbDataIn2.Text = "Ready";
                }
            }
            catch (Exception error)
            {
                MessageBox.Show(error.Message);
            }
        }
            //****************************************************** Button Routines ****************************************************************
            //

        int buttonPressed;
        int range;
        Int16 TmpM;
        Int16 TmpP;
        Int16 adjT;

        private void AdjustAnalogSettings(object sender, EventArgs e)
        {
            switch (buttonPressed)
            {
                case 0:
                    TmpM = (short)(TmpM - adjT);
                    if (TmpM < range && TmpM > 0)
                    {
                    }
                    else
                    {
                        TmpM = 1;
                    }
                    break;
                case 1:
                    TmpP = (short)(TmpP + adjT);
                    if (TmpP < range && TmpP > 0)
                    {
                    }
                    else
                    {
                        TmpP = (short)range;
                    }
                    break;
            }
        }

        //
        //****************************************************** Config Menus *******************************************************************
        //
        byte ValueSet;

        private void comboBoxGPKBstartMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            cfgInfoBox.Image = Properties.Resources.GamepadKeyboard;
            if (StartupSettingsFlag != 1)
            {
                cfgInfoBox.Visible = true;
                if (comboBoxGPKBstartMode.Text == "Keyboard")
                {
                    ValueSet = 0;
                }
                else
                {
                    ValueSet = 1;
                }
                ConfigSet[0] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        private void comboBoxIndMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            cfgInfoBox.Image = Properties.Resources.UseIndicator;
            if (StartupSettingsFlag != 1)
            {
                cfgInfoBox.Visible = true;
                if (comboBoxIndMode.Text == "ON at Rest")
                {
                    ValueSet = 1;
                }
                else if (comboBoxIndMode.Text == "OFF at Rest")
                {
                    ValueSet = 2;
                }
                else if (comboBoxIndMode.Text == "ON at Rest Inverted")
                {
                    ValueSet = 3;
                }
                else if (comboBoxIndMode.Text == "OFF at Rest Inverted")
                {
                    ValueSet = 4;
                }
                else
                {
                    ValueSet = 0;
                }
                ConfigSet[1] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        private void comboBoxBrightness_SelectedIndexChanged(object sender, EventArgs e)
        {
            cfgInfoBox.Image = Properties.Resources.IndicatorBrightness;
            if (StartupSettingsFlag != 1)
            {
                cfgInfoBox.Visible = true;
                ValueSet = Convert.ToByte(comboBoxBrightness.Text);
                ConfigSet[14] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        private void comboBoxSensor_SelectedIndexChanged(object sender, EventArgs e)
        {
            cfgInfoBox.Image = Properties.Resources.SensorType;
            if (StartupSettingsFlag != 1)
            {
                cfgInfoBox.Visible = true;
                if (comboBoxSensor.Text == "IR Sensor")
                {
                    ValueSet = 1;
                }
                else if (comboBoxSensor.Text == "Potentiometer")
                {
                    ValueSet = 2;
                }
                else if (comboBoxSensor.Text == "Disabled")
                {
                    ValueSet = 0;
                    btnCal.Enabled = false;
                }
                ConfigSet[11] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        private void comboBoxOrientation_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                cfgInfoBox.Visible = true;
                if (comboBoxOrientation.SelectedIndex == 0)
                {
                    cfgInfoBox.Image = Properties.Resources.vleft;
                    ValueSet = 0;
                }
                else if (comboBoxOrientation.SelectedIndex == 1)
                {
                    cfgInfoBox.Image = Properties.Resources.vcentF;
                    ValueSet = 1;
                }
                else if (comboBoxOrientation.SelectedIndex == 2)
                {
                    cfgInfoBox.Image = Properties.Resources.vcentB;
                    ValueSet = 2;
                }
                else if (comboBoxOrientation.SelectedIndex == 3)
                {
                    cfgInfoBox.Image = Properties.Resources.vright;
                    ValueSet = 3;
                }
                else if (comboBoxOrientation.SelectedIndex == 4)
                {
                    cfgInfoBox.Image = Properties.Resources.HcableL;
                    ValueSet = 4;
                }
                else if (comboBoxOrientation.SelectedIndex == 5)
                {
                    cfgInfoBox.Image = Properties.Resources.HcableF;
                    ValueSet = 5;
                }
                else if (comboBoxOrientation.SelectedIndex == 6)
                {
                    cfgInfoBox.Image = Properties.Resources.HcableR;
                    ValueSet = 6;
                }
                else if (comboBoxOrientation.SelectedIndex == 7)
                {
                    cfgInfoBox.Image = Properties.Resources.HcableB;
                    ValueSet = 7;
                }
                ConfigSet[3] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        private void comboBoxAccSensitivity_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                cfgInfoBox.Visible = true;
                cfgInfoBox.Image = Properties.Resources.Sensitivity;
                if (comboBoxAccSensitivity.Text == "Disabled")
                {
                    ValueSet = 0;
                }
                else
                {
                    char[] TrailerRemoved = { 'g' };
                    string gravTmp = comboBoxAccSensitivity.Text.Trim(TrailerRemoved);
                    ValueSet = Convert.ToByte(gravTmp);
                }
                ConfigSet[13] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        private void comboBoxTiltEnable_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                cfgInfoBox.Visible = true;
                cfgInfoBox.Image = Properties.Resources.TiltEnable;
                if (comboBoxTiltEnable.Text != "Enabled")
                {
                    ValueSet = 0;
                    radioButton4.Enabled = false;
                    radioButton5.Enabled = false;
                    radioButton6.Enabled = false;
                }
                else
                {
                    ValueSet = 1;
                    rtBoxTiltValX.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxTiltValX.Text = ConfigAnalog[4].ToString();
                    rtBoxTiltValY.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxTiltValY.Text = ConfigAnalog[5].ToString();
                    radioButton4.Enabled = true;
                    radioButton5.Enabled = true;
                    radioButton6.Enabled = true;
                    rtBoxTbtn.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxTbtn.Text = ConfigSet[12].ToString();
                }
                ConfigSet[6] = ValueSet;
                //SetUpdateImageVisible();
            }
        }
        private void comboBoxHatSw_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                cfgInfoBox.Visible = true;
                if (comboBoxHatSw.Text == "Enabled")
                {
                    ValueSet = 1;
                }
                else
                {
                    ValueSet = 0;
                }
                ConfigSet[8] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        private void cmBoxTrackB_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                cfgInfoBox.Visible = true;
                if (cmBoxTrackB.Text == "Enabled")
                {
                    ValueSet = 1;
                }
                else
                {
                    ValueSet = 0;
                }
                if (ValueSet != PrevConfigSet[15])
                {
                    ConfigSet[15] = ValueSet;
                }
                SetUpdateImageVisible();
            }
        }

        int analogChanged;
        int configChanged;
        int[] ConfigSetChanged = new int[32];
        int[] ConfigAnalogChanged = new int[32];
        private void SetUpdateImageVisible()
        {
            configChanged = 0;
            analogChanged = 0;
            for (int a = 0; a < 32; a++)
            {
                if (ConfigSet[a] != PrevConfigSet[a])
                {
                    ConfigSetChanged[a] = 1;
                }
                else
                {
                    ConfigSetChanged[a] = 0;
                }
                configChanged += ConfigSetChanged[a];
                if (ConfigAnalog[a] != PrevConfigAnalog[a])
                {
                    ConfigAnalogChanged[a] = 1;
                }
                else
                {
                    ConfigAnalogChanged[a] = 0;
                }
                analogChanged += ConfigAnalogChanged[a];
            }
            if (configChanged != 0 || analogChanged != 0)
            {
                picwarning.Visible = true;
                lblpicWarning2.Visible = true;
                rtbDataIn.Text = "Setting Changed Click Update to Send";
            }
            else
            {
                picwarning.Visible = false;
                lblpicWarning2.Visible = false;
                rtbDataIn.Text = "Ready";
            }
        }
        //
        //************************************************* Main Page Buttons ******************************************************************
        //
        private void btnFactoryReset_Click(object sender, EventArgs e)
        {
            FactoryReset();
        }
        private void btnReset2_Click(object sender, EventArgs e)
        {
            FactoryReset();
        }
        private void FactoryReset()
        {
            serialPort1.Write("F");    /// send code to set byte in firmware
            rtbDataIn.Text = "Factory Settings Restored";
            Task.Delay(1500).Wait();
            rtbDataIn.Text = "Ready";
        }
        private void btnClearData_Click(object sender, EventArgs e)
        {
            ClearData();
        }
        private void btnClearData2_Click(object sender, EventArgs e)
        {
            ClearData();
        }
        private void ClearData()
        {
            if (rtbDataIn.Text != "")
            {
                rtbDataIn.Text = "";
                rtbDataIn2.Text = "";
            }
        }
        private void btnReboot_Click(object sender, EventArgs e)
        {
            Reboot();
        }
        private void btnReboot2_Click(object sender, EventArgs e)
        {
            Reboot();
        }
        private void Reboot()
        {
            serialPort1.WriteLine("R");
            Task.Delay(10).Wait();
            btnDisconnect.PerformClick();
            rtbDataIn.Text = "Disconnecting and Rebooting";
            Task.Delay(6000).Wait();
            rtbDataIn.Text = " Attempting to Reconnect";
            Task.Delay(2500).Wait();
            btnAutoConnect.PerformClick();
            if (serialPort1.IsOpen)
            {
                rtbDataIn.Text = " Ready";
            }
            else
            {
                rtbDataIn.Text = "Please Reconnect to COM Port ";
            }
        }
        private void btnRefresh_Click_1(object sender, EventArgs e)
        {
            RefreshSettings();
        }
        private void btnRefresh2_Click(object sender, EventArgs e)
        {
            RefreshSettings();
        }
        private void RefreshSettings()
        {
            StartupSettingsFlag = 1;
            Count = 1;
            serialPort1.Write("E");
        }
        //
        //************************************************ UPDATE BUTTON *********************************************************************
        private void btnUpdateCfg_Click(object sender, EventArgs e)
        {
            UpdateCfg();
        }
        private void btnUpdate2_Click(object sender, EventArgs e)
        {
            UpdateCfg();
        }

        int updateFlag = 0;  // blocks 'Acks' from being displayed in message box 
        private void UpdateCfg()
        {
            updateFlag = 1;
            if (serialPort1.IsOpen)
            {
                
                if (configChanged == 0 && keysChanged == 0 && analogChanged == 0)
                {
                    rtbDataIn.Text = "No Changes to send";
                    rtbDataIn2.Text = "No Changes to send";
                    picwarning.Visible = false;
                    Task.Delay(1500).Wait();
                    rtbDataIn.Text = "Ready";
                    rtbDataIn2.Text = "Ready";
                }
                else
                {
                    if (configChanged != 0)
                    {
                        rtbDataIn.Text = "Sending Configuration Values";
                        rtbDataIn2.Text = "Sending Configuration Values";
                        serialPort1.Write("B" + ConfigSet[0] + ",");
                        for (int a = 1; a < 30; a++)
                        {
                            serialPort1.Write(ConfigSet[a] + ",");
                        }
                        serialPort1.WriteLine(ConfigSet[30] + "," + ConfigSet[31]);
                        for (int a = 0; a < 32; a++)
                        {
                            PrevConfigSet[a] = ConfigSet[a];
                            ConfigSetChanged[a] = 0;
                        }
                        Task.Delay(50).Wait();
                        DataIn = serialPort1.ReadLine();
                        if (DataIn.Trim() == "A")
                        {
                            rtbDataIn.Text = "Config Transmission Received";
                            rtbDataIn2.Text = "Config Transmission Received";
                            picwarning.Visible = false;
                            Task.Delay(500).Wait();
                            rtbDataIn.Text = "Ready";
                            configChanged = 0;
                        }
                        else
                        {
                            rtbDataIn.Text = "Config Transmission Error";
                            rtbDataIn2.Text = "Config Transmission Error";
                        }
                    }
                    if (keysChanged != 0)
                    {
                        rtbDataIn.Text = "Sending Keys Values";
                        rtbDataIn2.Text = "Sending Keys Values";
                        serialPort1.Write("C" + ConfigKeys[0] + ",");
                        for (int a = 1; a < 30; a++)
                        {
                            serialPort1.Write(ConfigKeys[a] + ",");
                        }
                        serialPort1.Write(ConfigKeys[30] + "," + ConfigKeys[31]);
                        Task.Delay(100).Wait();
                        DataIn = serialPort1.ReadLine();
                        if (DataIn.Trim() == "A")
                        {
                            rtbDataIn.Text = "Keys Transmission Received";
                            rtbDataIn2.Text = "Keys Transmission Received";
                            serialPort1.Write("L" + LabelCodesReceived[0] + ",");
                            for (int a = 1; a < 30; a++)
                            {
                                serialPort1.Write(LabelCodesReceived[a] + ",");
                            }
                            serialPort1.WriteLine(LabelCodesReceived[30] + "," + LabelCodesReceived[31]);
                            Task.Delay(100).Wait();
                            DataIn = serialPort1.ReadLine();
                        }
                        if (DataIn.Trim() == "A")
                        {
                            rtbDataIn.Text = "Label Transmission Received";
                            rtbDataIn2.Text = "Label Transmission Received";
                            picwarning.Visible = false;
                            lblpicWarning2.Visible = false;
                            Task.Delay(500).Wait();
                            rtbDataIn.Text = "Ready";
                            rtbDataIn2.Text = "Ready";
                            keysChanged = 0;
                        }
                        else
                        {
                            rtbDataIn.Text = "Keys Transmission Error";
                            rtbDataIn2.Text = "Keys Transmission Error";
                        }
                    }
                    if (analogChanged != 0)
                    {
                        rtbDataIn.Text = "Sending Accelerometer Values";
                        rtbDataIn2.Text = "Sending Accelerometer Values";
                        serialPort1.Write("I" + ConfigAnalog[0] + ",");
                        for (int a = 1; a < 4; a++)
                        {
                            serialPort1.Write(ConfigAnalog[a] + ",");
                        }
                        serialPort1.WriteLine(ConfigAnalog[4] + "," + ConfigAnalog[5]);
                        for (int a = 0; a < 32; a++)
                        {
                            PrevConfigAnalog[a] = ConfigAnalog[a];
                            ConfigAnalogChanged[a] = 0;
                        }
                        Task.Delay(50).Wait();
                        DataIn = serialPort1.ReadLine();
                        if (DataIn.Trim() == "A")
                        {
                            rtbDataIn.Text = "Analog Transmission Received";
                            rtbDataIn2.Text = "Analog Transmission Received";
                            picwarning.Visible = false;
                            Task.Delay(500).Wait();
                            rtbDataIn.Text = "Ready";
                            rtbDataIn2.Text = "Ready";
                            analogChanged = 0;
                        }
                        else
                        {
                            rtbDataIn.Text = "Analog Transmission Error";
                            rtbDataIn2.Text = "Analog Transmission Error";
                        }
                    }
                }

            }
            else
            {
                rtbDataIn.Text = "COM Port Error. ReConnect to Port";
                rtbDataIn2.Text = "COM Port Error. ReConnect to Port";
            }
            updateFlag = 0;
        }
        //
        //************************************************ PLUNGER Calibration & Deadzone ****************************************************
        //

        int plungerCalFlag = 0;

        private void btnCal_Click(object sender, EventArgs e)
        {
            plungerCalFlag = 1;
            rtbDataIn.Text = "Calibrating";
            serialPort1.WriteLine("P");
            Task.Delay(50).Wait();
            DataIn = serialPort1.ReadLine();
            DataIn = DataIn.Trim();
            switch (DataIn)
            {
                case "A":
                    rtbDataIn.Text = "Pull Plunger Out and Hold";
                    serialPort1.WriteLine("A");
                    Task.Delay(1000).Wait();
                    while (DataIn != "B")
                    {
                        DataIn = serialPort1.ReadLine();
                        DataIn = DataIn.Trim();
                        if (DataIn != "B")
                        {
                            rtBoxDraw.Text = DataIn;
                        }

                        Task.Delay(10).Wait();
                    }
                    rtbDataIn.Text = "Release";
                    Task.Delay(1500).Wait();
                    serialPort1.WriteLine("A");
                    while (DataIn != "C")
                    {
                        DataIn = serialPort1.ReadLine();
                        DataIn = DataIn.Trim();
                    }
                    rtbDataIn.Text = "Push Plunger In and Hold";
                    Task.Delay(100).Wait();
                    serialPort1.WriteLine("A");
                    Task.Delay(100).Wait();
                    while (DataIn != "D")
                    {
                        DataIn = serialPort1.ReadLine();
                        DataIn = DataIn.Trim();
                        if (DataIn != "D")
                        {
                            rtBoxPush.Text = DataIn;
                        }
                        Task.Delay(5).Wait();
                    }
                    rtbDataIn.Text = "Release";
                    Task.Delay(1000).Wait();
                    serialPort1.WriteLine("A");
                    while (DataIn != "F")
                    {
                        DataIn = serialPort1.ReadLine();
                        DataIn = DataIn.Trim();
                    }
                    plungerCalFlag = 0;
                    rtbDataIn.Text = "Ready";
                    break;
                default:
                    rtbDataIn.Text = "Communication ERROR";
                    plungerCalFlag = 0;
                    break;
            }
        }
        private void btnDzIncrement_Click(object sender, EventArgs e)
        {
            if (btnDzIncrement.Text == "1")
            {
                btnDzIncrement.Text = "10";
            }
            else
            {
                btnDzIncrement.Text = "1";
            }
        }
        private void btnDzM_Click(object sender, EventArgs e)
        {
            int DzTmpM = int.Parse(rtBoxDeadzone.Text);
            int adjM = int.Parse(btnDzIncrement.Text);
            DzTmpM = DzTmpM - adjM;
            rtBoxDeadzone.SelectionAlignment = HorizontalAlignment.Center;
            rtBoxDeadzone.Text = DzTmpM.ToString();
        }
        private void btnDzP_Click(object sender, EventArgs e)
        {
            int DzTmpP = int.Parse(rtBoxDeadzone.Text);
            int adjP = int.Parse(btnDzIncrement.Text);
            DzTmpP = DzTmpP + adjP;
            rtBoxDeadzone.SelectionAlignment = HorizontalAlignment.Center;
            rtBoxDeadzone.Text = DzTmpP.ToString();
        }
        private void rtBoxDeadzone_TextChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                ValueSet = Convert.ToByte(rtBoxDeadzone.Text);
                ConfigSet[2] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        //
        //************************************************ Nudging Adjustments ***************************************************************
        //
        int selectedSetting;

        private void btnIncrement_Click_1(object sender, EventArgs e)
        {
            switch (selectedSetting)
            {
                case 1:
                    if (btnIncrement.Text == "1")
                    {
                        btnIncrement.Text = "10";
                    }
                    else
                    {
                        btnIncrement.Text = "1";
                    }
                    break;
                case 2:
                    if (btnIncrement.Text == "1")
                    {
                        btnIncrement.Text = "10";
                    }
                    else
                    {
                        btnIncrement.Text = "1";
                    }
                    break;
                case 7:
                    if (btnIncrement.Text == "1")
                    {
                        btnIncrement.Text = "10";
                    }
                    else
                    {
                        btnIncrement.Text = "1";
                    }
                    break;
                default:
                    if (btnIncrement.Text == "1")
                    {
                        btnIncrement.Text = "10";
                    }
                    else if (btnIncrement.Text == "10")
                    {
                        btnIncrement.Text = "100";
                    }
                    else if (btnIncrement.Text == "100")
                    {
                        btnIncrement.Text = "1000";
                    }
                    else
                    {
                        btnIncrement.Text = "1";
                    }
                    break;
            }
        }
        private void btnMinus_Click(object sender, EventArgs e)
        {
            buttonPressed = 0;
            switch (selectedSetting)
            {
                case 1:
                    TmpM = Int16.Parse(rtBoxXgain.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtBoxXgain.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxXgain.Text = TmpM.ToString();
                    break;
                case 2:
                    TmpM = Int16.Parse(rtBoxYgain.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtBoxYgain.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxYgain.Text = TmpM.ToString();
                    break;
                case 3:
                    TmpM = Int16.Parse(rtbXdZone.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtbXdZone.SelectionAlignment = HorizontalAlignment.Center;
                    rtbXdZone.Text = TmpM.ToString();
                    break;
                case 4:
                    TmpM = Int16.Parse(rtbYdZone.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtbYdZone.SelectionAlignment = HorizontalAlignment.Center;
                    rtbYdZone.Text = TmpM.ToString();
                    break;
                case 5:
                    TmpM = Int16.Parse(rtBoxTiltValX.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtBoxTiltValX.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxTiltValX.Text = TmpM.ToString();
                    break;
                case 6:
                    TmpM = Int16.Parse(rtBoxTiltValY.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtBoxTiltValY.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxTiltValY.Text = TmpM.ToString();
                    break;
                case 7:
                    TmpM = Int16.Parse(rtBoxTbtn.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtBoxTbtn.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxTbtn.Text = TmpM.ToString();
                    break;
                default:
                    break;
            }
        }
        private void btnPlus_Click(object sender, EventArgs e)
        {
            buttonPressed = 1;
            switch (selectedSetting)
            {
                case 1:
                    TmpP = Int16.Parse(rtBoxXgain.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtBoxXgain.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxXgain.Text = TmpP.ToString();
                    break;
                case 2:
                    TmpP = Int16.Parse(rtBoxYgain.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtBoxYgain.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxYgain.Text = TmpP.ToString();
                    break;
                case 3:
                    TmpP = Int16.Parse(rtbXdZone.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtbXdZone.SelectionAlignment = HorizontalAlignment.Center;
                    rtbXdZone.Text = TmpP.ToString();
                    break;
                case 4:
                    TmpP = Int16.Parse(rtbYdZone.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtbYdZone.SelectionAlignment = HorizontalAlignment.Center;
                    rtbYdZone.Text = TmpP.ToString();
                    break;
                case 5:
                    TmpP = Int16.Parse(rtBoxTiltValX.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtBoxTiltValX.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxTiltValX.Text = TmpP.ToString();
                    break;
                case 6:
                    TmpP = Int16.Parse(rtBoxTiltValY.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtBoxTiltValY.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxTiltValY.Text = TmpP.ToString();
                    break;
                case 7:
                    TmpP = Int16.Parse(rtBoxTbtn.Text);
                    adjT = Int16.Parse(btnIncrement.Text);
                    this.Invoke(new EventHandler(AdjustAnalogSettings));
                    rtBoxTbtn.SelectionAlignment = HorizontalAlignment.Center;
                    rtBoxTbtn.Text = TmpP.ToString();
                    break;
                default:
                    break;
            }
        }
        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                picBoxSettings.Visible = true;
            }
            picBoxSettings.Image = Properties.Resources.xyGain;
            selectedSetting = 1;
            range = 255;
        }
        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                picBoxSettings.Visible = true;
            }
            picBoxSettings.Image = Properties.Resources.xyGain;
            selectedSetting = 2;
            range = 255;
        }
        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                picBoxSettings.Visible = true;
            }
            picBoxSettings.Image = Properties.Resources.xyDeadzone;
            selectedSetting = 3;
            range = 32767;
        }
        private void YDeadzone_CheckedChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                picBoxSettings.Visible = true;
            }
            picBoxSettings.Image = Properties.Resources.xyDeadzone;
            selectedSetting = 4;
            range = 32767;
        }
        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                picBoxSettings.Visible = true;
            }
            picBoxSettings.Image = Properties.Resources.tiltThreshold;
            selectedSetting = 5;
            range = 32767;
        }
        private void radioButton6_CheckedChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                picBoxSettings.Visible = true;
            }
            picBoxSettings.Image = Properties.Resources.tiltThreshold;
            selectedSetting = 6;
            range = 32767;
        }
        private void radioButton5_CheckedChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                picBoxSettings.Visible = true;
            }
            picBoxSettings.Image = Properties.Resources.tiltButton;
            selectedSetting = 7;
            range = 32;
        }
        private void rtBoxXgain_TextChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                ValueSet = Convert.ToByte(rtBoxXgain.Text);
                ConfigSet[4] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        private void rtBoxYgain_TextChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                ValueSet = Convert.ToByte(rtBoxYgain.Text);
                ConfigSet[5] = ValueSet;
                SetUpdateImageVisible();
            }
        }

        Int16 AnValSet;

        private void rtbXdZone_TextChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                AnValSet = Int16.Parse(rtbXdZone.Text);
                ConfigAnalog[2] = AnValSet;
                SetUpdateImageVisible();
            }
        }
        private void rtbYdZone_TextChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                AnValSet = Int16.Parse(rtbYdZone.Text);
                ConfigAnalog[3] = AnValSet;
                SetUpdateImageVisible();
            }
        }
        private void rtBoxTiltValX_TextChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1 && ConfigSet[6] != 0)
            {
                AnValSet = Int16.Parse(rtBoxTiltValX.Text);
                ConfigAnalog[4] = AnValSet;
                SetUpdateImageVisible();
            }
        }
        private void rtBoxTiltValY_TextChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1 && ConfigSet[6] != 0)
            {
                AnValSet = Int16.Parse(rtBoxTiltValY.Text);
                ConfigAnalog[5] = AnValSet;
                SetUpdateImageVisible();
            }
        }
        private void rtBoxTbtn_TextChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                ValueSet = Convert.ToByte(rtBoxTbtn.Text);
                ConfigSet[12] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        private void btnTestHW_Click_1(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                System.Diagnostics.Process.Start("joy.cpl");
                Task.Delay(1000).Wait();
                SendKeys.Send("%{P}");
                Task.Delay(1000).Wait();
            }
            else
            { }
        }
        //
        //************************************************ KEYS TAB **************************************************************************
        //
        //
        private void rtbLBPos_TextChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                ValueSet = Convert.ToByte(rtbLBPos.Text);
                ConfigSet[7] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        private void rtbLBval_TextChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                ValueSet = Convert.ToByte(rtbLBval.Text);
                ConfigSet[10] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        private void rtbCDpos_TextChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                ValueSet = Convert.ToByte(rtbCDpos.Text);
                ConfigSet[16] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        private void rtbCDval_TextChanged(object sender, EventArgs e)
        {
            if (StartupSettingsFlag != 1)
            {
                ValueSet = Convert.ToByte(rtbCDval.Text);
                ConfigSet[9] = ValueSet;
                SetUpdateImageVisible();
            }
        }
        private void lblButton1_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 1;
            ButtonPreselect();
        }
        private void lblButton2_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 2;
            ButtonPreselect();
        }
        private void lblButton3_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 3;
            ButtonPreselect();
        }
        private void lblButton4_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 4;
            ButtonPreselect();
        }
        private void lblButton5_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 5;
            ButtonPreselect();
        }
        private void lblButton6_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 6;
            ButtonPreselect();
        }
        private void lblButton7_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 7;
            ButtonPreselect();
        }
        private void lblButton8_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 8;
            ButtonPreselect();
        }
        private void lblButton9_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 9;
            ButtonPreselect();
        }
        private void lblButton10_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 10;
            ButtonPreselect();
        }
        private void lblButton11_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 11;
            ButtonPreselect();
        }
        private void lblButton12_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 12;
            ButtonPreselect();
        }
        private void lblButton13_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 13;
            ButtonPreselect();
        }
        private void lblButton14_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 14;
            ButtonPreselect();
        }
        private void lblButton15_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 15;
            ButtonPreselect();
        }
        private void lblButton16_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 16;
            ButtonPreselect();
        }
        private void lblButton17_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 17;
            ButtonPreselect();
        }
        private void lblButton18_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 18;
            ButtonPreselect();
        }
        private void lblButton19_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 19;
            ButtonPreselect();
        }
        private void lblButton20_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 20;
            ButtonPreselect();
        }
        private void lblButton21_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 21;
            ButtonPreselect();
        }
        private void lblButton22_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 22;
            ButtonPreselect();
        }
        private void lblButton23_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 23;
            ButtonPreselect();
        }
        private void lblButton24_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 24;
            ButtonPreselect();
        }
        private void lblButton25_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 25;
            ButtonPreselect();
        }
        private void lblButton26_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 26;
            ButtonPreselect();
        }
        private void lblButton27_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 27;
            ButtonPreselect();
        }
        private void lblButton28_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 28;
            ButtonPreselect();
        }
        private void lblButton29_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 29;
            ButtonPreselect();
        }
        private void lblButton30_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 30;
            ButtonPreselect();
        }
        private void lblButton31_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 31;
            ButtonPreselect();
        }
        private void lblButton32_Click(object sender, EventArgs e)
        {
            ButtonPreSelected = 32;
            ButtonPreselect();
        }

        int ButtonPreSelected;

        private void ButtonPreselect()
        {
            Label[] LabelUse = new Label[] { lblUse1, lblUse2, lblUse3, lblUse4, lblUse5, lblUse6, lblUse7, lblUse8,
                                                 lblUse9, lblUse10, lblUse11, lblUse12, lblUse13, lblUse14, lblUse15, lblUse16,
                                                 lblUse17, lblUse18, lblUse19, lblUse20, lblUse21, lblUse22, lblUse23, lblUse24,
                                                 lblUse25, lblUse26, lblUse27, lblUse28, lblUse29, lblUse30, lblUse31, lblUse32 };
            Label[] KeyUse = new Label[] { lblKey1, lblKey2, lblKey3, lblKey4, lblKey5, lblKey6, lblKey7, lblKey8,
                                               lblKey9, lblKey10, lblKey11, lblKey12, lblKey13, lblKey14, lblKey15, lblKey16,
                                               lblKey17, lblKey18, lblKey19, lblKey20, lblKey21, lblKey22, lblKey23, lblKey24,
                                               lblKey25, lblKey26, lblKey27, lblKey28, lblKey29, lblKey30, lblKey31, lblKey32 };
             picBoxError.Visible = false;
             cmBoxButton.Text = ButtonPreSelected.ToString();
             cmBoxButtonUse.Text = LabelUse[ButtonPreSelected - 1].Text;
             cmBoxKey.Text = KeyUse[ButtonPreSelected - 1].Text;
        }
        private void cmBoxButton_SelectedIndexChanged(object sender, EventArgs e)
        {
            int ButtonSelected = int.Parse(cmBoxButton.Text);
            cmBoxButtonUse.Text = ""; cmBoxButtonUse.Enabled = true;
            cmBoxKey.Text = ""; cmBoxKey.Enabled = true;
            btnApply.Enabled = true;
            picBoxError.Visible = false;
        }

        int keysChanged;   // flags the update button routine to send key array to plunger
        int BtnTmp;

        private void btnApply_Click_1(object sender, EventArgs e)
        {
            Label[] LabelUse = new Label[] { lblUse1, lblUse2, lblUse3, lblUse4, lblUse5, lblUse6, lblUse7, lblUse8,
                                             lblUse9, lblUse10, lblUse11, lblUse12, lblUse13, lblUse14, lblUse15, lblUse16,
                                             lblUse17, lblUse18, lblUse19, lblUse20, lblUse21, lblUse22, lblUse23, lblUse24,
                                             lblUse25, lblUse26, lblUse27, lblUse28, lblUse29, lblUse30, lblUse31, lblUse32 };
            Label[] KeyUse = new Label[] { lblKey1, lblKey2, lblKey3, lblKey4, lblKey5, lblKey6, lblKey7, lblKey8,
                                           lblKey9, lblKey10, lblKey11, lblKey12, lblKey13, lblKey14, lblKey15, lblKey16,
                                           lblKey17, lblKey18, lblKey19, lblKey20, lblKey21, lblKey22, lblKey23, lblKey24,
                                           lblKey25, lblKey26, lblKey27, lblKey28, lblKey29, lblKey30, lblKey31, lblKey32 };
            BtnTmp = int.Parse(cmBoxButton.Text);
            BtnTmp = BtnTmp - 1;
            LabelUse[BtnTmp].Text = cmBoxButtonUse.Text;
            KeyUse[BtnTmp].Text = cmBoxKey.Text;
            SortKeyEntries();
            keysChanged = 1;
            picwarning.Visible = true;
            lblpicWarning2.Visible = true;
        }
        private void SortKeyEntries()
        {
            byte[] AsciiCodes =  // 73 entries
            {
                0x00,
                0xE1,
                0xE0,
                0xE2,
                0xE3,
                0xE5,
                0xE4,
                0xE6,
                0xE7,
                0x04,
                0x05,
                0x06,
                0x07,
                0x08,
                0x09,
                0x0A,
                0x0B,
                0x0C,
                0x0D,
                0x0E,
                0x0F,
                0x10,
                0x11,
                0x12,
                0x13,
                0x14,
                0x15,
                0x16,
                0x17,
                0x18,
                0x19,
                0x1A,
                0x1B,
                0x1C,
                0x1D,
                0x1E,
                0x1F,
                0x20,
                0x21,
                0x22,
                0x23,
                0x24,
                0x25,
                0x26,
                0x27,
                0x28,
                0x29,
                0x2B,
                0x2C,
                0x2D,
                0x2E,
                0x49,
                0x4A,
                0x4B,
                0x4C,
                0x4D,
                0x4E,
                0x4F,
                0x50,
                0x51,
                0x52,
                0x3A,
                0x3B,
                0x3C,
                0x3D,
                0x3E,
                0x3F,
                0x40,
                0x41,
                0x42,
                0x43,
                0x44,
                0x45
            };
            string[] AsciiNames =  // 73 entries
            {
                "NOT ASSIGNED",
                "SHIFT_LEFT",
                "CRTL_LEFT",
                "ALT_LEFT",
                "GUI_LEFT",
                "SHIFT_RIGHT",
                "CTRL_RIGHT",
                "ALT_RIGHT",
                "GUI_RIGHT",
                "a",
                "b",
                "c",
                "d",
                "e",
                "f",
                "g",
                "h",
                "i",
                "j",
                "k",
                "l",
                "m",
                "n",
                "o",
                "p",
                "q",
                "r",
                "s",
                "t",
                "u",
                "v",
                "w",
                "x",
                "y",
                "z",
                "1",
                "2",
                "3",
                "4",
                "5",
                "6",
                "7",
                "8",
                "9",
                "0",
                "ENTER",
                "ESCAPE",
                "TAB",
                "SPACE",
                "MINUS",
                "EQUAL",
                "INSERT",
                "HOME",
                "PAGE_UP",
                "DELETE",
                "END",
                "PAGE_DOWN",
                "ARROW_RIGHT",
                "ARROW_LEFT",
                "ARROW_DOWN",
                "ARROW_UP",
                "F1",
                "F2",
                "F3",
                "F4",
                "F5",
                "F6",
                "F7",
                "F8",
                "F9",
                "F10",
                "F11",
                "F12"
            };
            string[] LabelCodeNames = // 32 entries
            {
                "Not Assigned",
                "L Flipper",
                "R Flipper",
                "L Magnasave",
                "R Magnasave",
                "Start",
                "Exit",
                "Buy In",
                "Coin 1",
                "Coin 2",
                "Launch Ball",
                "Fire",
                "Coin Door",
                "Pause",
                "Volume Up",
                "Volume Down",
                "Menu Up",
                "Menu Down",
                "Menu Enter",
                "Menu Cancel",
                "L Nudge",
                "R Nudge",
                "F Nudge",
                "Tilt",
                "HSW1 U",
                "HSW1 R",
                "HSW1 D",
                "HSW1 L",
                "HSW2 U",
                "HSW2 R",
                "HSW2 D",
                "HSW2 L"
            };
            Byte[] LabelCodes = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
                                  0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};


            //*********************************************** Startup Read and Assign *******************************************************
            //
            Label[] LabelUse = new Label[] { lblUse1, lblUse2, lblUse3, lblUse4, lblUse5, lblUse6, lblUse7, lblUse8,
                                                 lblUse9, lblUse10, lblUse11, lblUse12, lblUse13, lblUse14, lblUse15, lblUse16,
                                                 lblUse17, lblUse18, lblUse19, lblUse20, lblUse21, lblUse22, lblUse23, lblUse24,
                                                 lblUse25, lblUse26, lblUse27, lblUse28, lblUse29, lblUse30, lblUse31, lblUse32 };
            Label[] KeyUse = new Label[] { lblKey1, lblKey2, lblKey3, lblKey4, lblKey5, lblKey6, lblKey7, lblKey8,
                                               lblKey9, lblKey10, lblKey11, lblKey12, lblKey13, lblKey14, lblKey15, lblKey16,
                                               lblKey17, lblKey18, lblKey19, lblKey20, lblKey21, lblKey22, lblKey23, lblKey24,
                                               lblKey25, lblKey26, lblKey27, lblKey28, lblKey29, lblKey30, lblKey31, lblKey32 };
            if (StartupSettingsFlag == 1)
            {
                int KeysAvailable = 32;
                for (int buttonNumber = 0; buttonNumber < KeysAvailable; buttonNumber++)
                {
                    if (ConfigKeys[buttonNumber] != 0x00)
                    {
                        for (int numberOfCodes = 0; numberOfCodes < 32; numberOfCodes++)
                        {
                            if (LabelCodesReceived[buttonNumber] == LabelCodes[numberOfCodes])
                            {
                                LabelUse[buttonNumber].Text = LabelCodeNames[numberOfCodes];
                            }
                        }
                        for (int numberOfnames = 0; numberOfnames < 73; numberOfnames++)
                        {
                            if (AsciiCodes[numberOfnames] == ConfigKeys[buttonNumber])
                            {
                                KeyUse[buttonNumber].Text = AsciiNames[numberOfnames];
                                numberOfnames = 72;  // break out of the loop
                            }
                        }

                    }
                    else
                    {
                        KeyUse[buttonNumber].Text = "NOT ASSIGNED";
                        LabelUse[buttonNumber].Text = "Not Assigned";
                    }
                }
                picBoxError.Visible = false;
                cmBoxButton.Text = "1";
                cmBoxButtonUse.Text = LabelUse[0].Text;
                cmBoxKey.Text = KeyUse[0].Text;
            }
            //*********************************************** Key Assignments Apply Button Clicked *******************************************
            //
            else
            {
                if (cmBoxButton.Text != "" && cmBoxKey.Text != "" && cmBoxButtonUse.Text != "")
                {

                    int index = cmBoxButtonUse.SelectedIndex;
                    LabelCodesReceived[BtnTmp] = LabelCodes[index];
                    for (int numberOfnames = 0; numberOfnames < 73; numberOfnames++)
                    {
                        if (AsciiNames[numberOfnames] == cmBoxKey.Text)
                        {
                            ConfigKeys[BtnTmp] = AsciiCodes[numberOfnames];
                            if (cmBoxButtonUse.Text == "Launch Ball")
                            {
                                rtbLBPos.Text = BtnTmp.ToString();
                                ConfigSet[7] = (byte)BtnTmp;
                                rtbLBval.Text = AsciiNames[numberOfnames];
                                ConfigSet[10] = AsciiCodes[numberOfnames];

                            }
                            else if (cmBoxButtonUse.Text == "Coin Door")
                            {
                                rtbLBPos.Text = BtnTmp.ToString();
                                ConfigSet[16] = (byte)BtnTmp;
                                rtbLBval.Text = AsciiNames[numberOfnames];
                                ConfigSet[9] = AsciiCodes[numberOfnames];

                            }
                            numberOfnames = 72;  // break out of the loop
                        }
                    }
                    keysChanged = 1;
                }
                else
                {
                    rtbDataIn2.Text = "INVALID SELECTION";
                    //keysChanged = 0;
                }
            }
        }

        int ButtonTest;  // blocks the 'acks' from being displayed in message box

        private void btnTestButtons_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                if (ButtonTest != 1)
                {
                    ButtonTest = 1;
                    btnTestButtons.Text = "STOP";
                    rtbDataIn.Text = "Button Test Running";
                    rtbDataIn2.Text = "Button Test Running";
                    rtbDataIn3.Text = "Button Test Running";
                }
                else
                {
                    btnTestButtons.Text = "TEST";
                    rtbDataIn.Text = "Button Test Stopped";
                    rtbDataIn2.Text = "Button Test Stopped";
                    rtbDataIn3.Text = "Button Test Stopped";
                    Task.Delay(1000).Wait();
                    rtbDataIn.Text = "Ready";
                    rtbDataIn2.Text = "Ready";
                    rtbDataIn3.Text = "Ready";
                    ButtonTest = 0;
                }
                serialPort1.WriteLine("T");
            }
            else
            {
                rtbDataIn2.Text = "CONTROLLER NOT CONNECTED";
            }
        }
        private void ButtonTestRoutine()
        {
            Label[] LabelInd = new Label[] { lblButton1, lblButton2, lblButton3, lblButton4, lblButton5, lblButton6, lblButton7, lblButton8,
                                                 lblButton9, lblButton10, lblButton11, lblButton12, lblButton13, lblButton14, lblButton15, lblButton16,
                                                 lblButton17, lblButton18, lblButton19, lblButton20, lblButton21, lblButton22, lblButton23, lblButton24,
                                                 lblButton25, lblButton26, lblButton27, lblButton28, lblButton29, lblButton30, lblButton31, lblButton32 };
            DataIn = DataIn.Trim();
            char[] LeaderRemoved = { 'T' };
            DataIn = DataIn.Trim(LeaderRemoved);
            string[] OutData = DataIn.Split(',');
            for (int a = 0; a < 32; a++)
            {
                if (OutData[a] == "0")
                {
                    LabelInd[a].BackColor = Color.Transparent;
                }
                else
                {
                    LabelInd[a].BackColor = Color.Red;
                }
            }
        }
        //
        //************************************************ DIAGNOSTICS TAB *******************************************************************
        //

        int TimeLeftRunning;

        private void TestTimer_Tick(object sender, EventArgs e)
        {
            if (TimeLeftRunning > 0)
            {
                TimeLeftRunning = TimeLeftRunning - 1;
            }
            else
            {
                TestTimer.Stop();
                btnAutoConnect.PerformClick();
                TimeLeftRunning = 1000;
            }
        }

        int reportFlag = 0;   // blocks 'Acks' from being displayed in message box

        private void btnRequestInfo_Click(object sender, EventArgs e)
        {
            reportFlag = 1;
            rtbDataIn3.Text = "Retrieving Plunger Settings ... Please Wait";
            Task.Delay(500).Wait();
            rtbDataIn3.Text = "";
            serialPort1.WriteLine("J");    /// Requesting Data Report
            Task.Delay(350).Wait();
            DataIn = serialPort1.ReadExisting();
            rtbDataIn3.Text = DataIn;
            reportFlag = 0;
        }
        private void button2_Click(object sender, EventArgs e)
        {
            if (rtbDataIn3.Text != "")
            {
                rtbDataIn3.Text = "";
            }
        }
        private void btnVerify_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                VerifiedFlag = 0;
                serialPort1.WriteLine("V");
                Task.Delay(100).Wait();
                VerifyFirmware();
                rtbManufacturerD.Text = rtbManufacturer.Text;
                rtbVIDD.Text = rtbVID.Text;
                rtbPIDD.Text = rtbPID.Text;
                rtbREVD.Text = rtbREV.Text;
            }
            else
            {
                rtbDataIn3.Text = "COM Port NOT CONNECTED.  Connect and retry.";
            }

        }
        private void btnDiagsCP_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                System.Diagnostics.Process.Start("joy.cpl");
                Task.Delay(1000).Wait();
                SendKeys.Send("%{P}");
                Task.Delay(1000).Wait();
            }
            else
            { }
        }
        private void btnCheckUpdates_Click(object sender, EventArgs e)
        {
            if (btnCheckUpdates.Text == "CHECK ONLINE FOR UPDATES")
            {
                UpdateCheck();
            }
            else
            {
                this.Invoke(new EventHandler(DownloadUpdate));
            }
        }
        private void UpdateCheck()
        {
            rtbDataIn3.Text = "Connecting to Server";
            WebClient CheckUpdate = new WebClient();
            System.IO.Stream stream = CheckUpdate.OpenRead("https://www.virtuapin.net/Plunger_Kit/Revision.ver");
            System.IO.StreamReader reader = new System.IO.StreamReader(stream);
            String CompleteRead = reader.ReadToEnd();
            string[] Revision = CompleteRead.Split(',');
            Revision[1] = Revision[1].Trim();
            if (Revision[0] != rtbREV.Text || Revision[1] != SWrevision)
            {
                label39.Visible = true;
                label44.Visible = true;
                lblFwUp.Visible = true;
                lblFW2.Visible = true;
                label41.Visible = true;
                label42.Visible = true;
                lblSwUp.Visible = true;
                lblSW2.Visible = true;
                btnCheckUpdates.BackColor = Color.Green;
                btnCheckUpdates.Text = "UPDATE AVAILABLE CLICK TO DOWNLOAD";
                rtbDataIn3.Text = "Update Available for Download";
                if (Revision[0] != rtbREV.Text)
                {
                    lblFwUp.Text = "YES";
                    lblFW2.Text = "YES";
                }
                else
                {
                    lblFwUp.Text = "NO";
                    lblFW2.Text = "NO";
                }
                if (Revision[1] != SWrevision)
                {
                    lblSwUp.Text = "YES";
                    lblSW2.Text = "YES";
                }
                else
                {
                    lblSwUp.Text = "NO";
                    lblSW2.Text = "NO";
                }
            }
            else
            {
                rtbDataIn3.Text = "Software Up To Date";
                Task.Delay(2000).Wait();
                rtbDataIn3.Text = "Ready";
            }
        }
        private void DownloadUpdate(object sender, EventArgs e)
        {
            var folderBrowserDialog1 = new FolderBrowserDialog();
            DialogResult result = folderBrowserDialog1.ShowDialog();
            if (result == DialogResult.OK)
            {
                string savePath = folderBrowserDialog1.SelectedPath;
                string filename = @"\VirtuaPinUpdater.zip";
                btnCheckUpdates.BackColor = Color.Maroon;
                btnCheckUpdates.Text = "CHECK ONLINE FOR UPDATES";
                progressBar.Visible = true;
                label37.Visible = true;
                rtbDataIn3.Text = "Ready";
                WebClient wc = new WebClient();
                wc.DownloadProgressChanged += wc_DownloadProgressChanged;
                wc.DownloadFileAsync(new System.Uri("https://www.virtuapin.net/Plunger_Kit/VirtuaPinUpdater.zip"), savePath + filename);
                lblDownloadInf.Visible = true;
                lblDownloadInf.Text = "File Saved to:  " + savePath + filename;
            }
        }
        void wc_DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
        {
            progressBar.Value = e.ProgressPercentage;
            if (progressBar.Value == 100)
            {
                label37.Text = "Download Complete";
            }
        }
    }
}