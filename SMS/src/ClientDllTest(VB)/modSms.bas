Attribute VB_Name = "modSms"
Option Explicit


Public Const MESSAGE_TYPE_CONNECT = 1
Public Const MESSAGE_TYPE_SEND_RETURN = 2
Public Const MESSAGE_TYPE_RECEIVE = 3
Public Const MESSAGE_TYPE_SERVICE_STOP = 4


Public Declare Function connect_to_server Lib "SmsClientCtrl" (ByVal lpServerIP As String, ByVal nPort As Long, ByVal lpName As String, ByVal lpPassword As String) As Long
Public Declare Function disconnect Lib "SmsClientCtrl" () As Long
Public Declare Function send_message Lib "SmsClientCtrl" (ByVal lpTel As String, ByVal lpContent As String, ByVal lpTag As String) As Long
Public Declare Sub set_callback Lib "SmsClientCtrl" (ByVal callback As Long)
Public Declare Function get_last_error_code Lib "SmsClientCtrl" () As Long


Public Const CLIENT_ERROR_CREATE_SOCKET = 101
Public Const CLIENT_ERROR_IP_ADDRESS = 102
Public Const CLIENT_ERROR_CONNECT = 103
Public Const CLIENT_ERROR_CREATE_THREAD = 104
Public Const CLIENT_ERROR_CREATE_AGENT = 105
Public Const CLIENT_ERROR_ACCOUNT_ALREADY_USED = 106
Public Const CLIENT_ERROR_ACCOUNT_NOTEXIST = 107
Public Const CLIENT_ERROR_NOT_CONNECT = 108
Public Const CLIENT_ERROR_BUSY_SENDING = 109



Sub SmsCallbackProc(ByVal nMsgType As Long, ByVal lpTel As String, ByVal lpContent As String, ByVal lpTag As String, ByVal bOK As Long)
    
    Dim strText As String
    
    
    
    Select Case nMsgType
    
        Case MESSAGE_TYPE_CONNECT
            If (bOK <> 0) Then
                strText = "连接成功"
            Else
                strText = "连接失败，错误码:" & CStr(get_last_error_code())
            End If
        
        Case MESSAGE_TYPE_SEND_RETURN
            If (bOK <> 0) Then
                strText = "发送成功，"
            Else
                strText = "发送失败，"
            End If
            lpTel = StrConv(lpTel, vbUnicode)
            lpContent = StrConv(lpContent, vbUnicode)
            lpTag = StrConv(lpTag, vbUnicode)
            strText = strText & "tel:" & lpTel & ", content:" & lpContent
            
            
        Case MESSAGE_TYPE_RECEIVE
            lpTel = StrConv(lpTel, vbUnicode)
            lpContent = StrConv(lpContent, vbUnicode)
            lpTag = StrConv(lpTag, vbUnicode)
             strText = "接收到新短信，" & "tel:" & lpTel & ", content:" & lpContent
        
        Case MESSAGE_TYPE_SERVICE_STOP
            strText = "短信服务停止或网络故障"
        
    End Select
    
    MsgBox strText, vbInformation + vbOKOnly, "提示信息"
    
End Sub


