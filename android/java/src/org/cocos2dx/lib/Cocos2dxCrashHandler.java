package org.cocos2dx.lib;
import java.io.File;
import java.io.IOException;
import java.text.MessageFormat;

import org.cocos2dx.lib.R;

import com.qihoo.linker.logcollector.capture.LogFileStorage;
import com.qihoo.linker.logcollector.upload.HttpManager;
import com.qihoo.linker.logcollector.upload.HttpParameters;
import com.qihoo.linker.logcollector.upload.UploadLogManager;
import com.qihoo.linker.logcollector.utils.Constants;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.Toast;

public class Cocos2dxCrashHandler extends Activity

{

         public static final String TAG = "Cocos2dxCrashHandler";

         protected void onCreate(Bundle state)

         {

                   super.onCreate(state);

                   setTitle(R.string.crashtitle);

                   setContentView(R.layout.crashhandler);

                   final Button b = (Button)findViewById(R.id.report),

                               c = (Button)findViewById(R.id.close);

                   final CheckBox cl = (CheckBox)findViewById(R.id.forget);

                   b.setOnClickListener(new View.OnClickListener(){public void onClick(View v){

                            final ProgressDialog progress = new ProgressDialog(Cocos2dxCrashHandler.this);

                            progress.setMessage(getString(R.string.getting_log));

                            progress.setIndeterminate(true);

                            progress.setCancelable(false);

                            progress.show();

                            final AsyncTask task = new AsyncTask<Void,Void,Void>(){

                                     String log;

                                     Process process;

                                     @Override

                                     protected Void doInBackground(Void... v) {

                                               try {

//                                                        process = Runtime.getRuntime().exec(new String[]{"logcat","-d","-v","threadtime"});
                                                        process = Runtime.getRuntime().exec(new String[]{"logcat","-d","-t","500","-v","threadtime"});  

                                                        log = Cocos2dxActivity.readAllOf(process.getInputStream());

                                               } catch (IOException e) {

                                                        e.printStackTrace();

                                                        Toast.makeText(Cocos2dxCrashHandler.this, e.toString(), Toast.LENGTH_LONG).show();

                                               }

                                               return null;

                                     }

                                     @Override

                                     protected void onCancelled() {

                                               process.destroy();

                                     }

                                     @Override
                                     

                                     protected void onPostExecute(Void v) {

                                    	 Log.i(TAG, "************************************************************************************");
                                    	 Log.i(TAG, log);
                                    	 Log.i(TAG, "************************************************************************************");
                                    	 
                                    	 LogFileStorage.getInstance(Cocos2dxCrashHandler.this).saveLogFile2Internal(log);
                                 		if(Constants.DEBUG)
                                 		{
                                 			LogFileStorage.getInstance(Cocos2dxCrashHandler.this).saveLogFile2SDcard(log, true);
                                 		}
                                 		HttpParameters mParams = new HttpParameters();
//                                 		mParams.add("key1", "value1");
                                 		
                                 		File logFile = LogFileStorage.getInstance(Cocos2dxCrashHandler.this).getUploadLogFile();
                            			if(logFile != null){
                            			try {
                                       	 Log.i(TAG, "77777777777777777777777777777777777777777777777777");

                            				String result = HttpManager.uploadFile(Cocos2dxActivity.UPLOAD_URL, mParams, logFile);
                            				if(result != null){
                            					Log.i(TAG, "777777777777777777777777777777777777777777777777771");
                            					LogFileStorage.getInstance(Cocos2dxCrashHandler.this).deleteUploadLogFile();
                            				}
                            				Log.i(TAG, "777777777777777777777777777777777777777777777777772");
                            			} catch (IOException e) {
                            				// TODO Auto-generated catch block
                            				e.printStackTrace();
                            			}finally{
                            			}
                            			}
                            			
                            			
                                 		
//                                 		HttpParameters mParams = new HttpParameters();
//                                 		mParams.add("key1", "value1");
//                                 		UploadLogManager.getInstance(Cocos2dxCrashHandler.this).uploadLogFile(Cocos2dxActivity.UPLOAD_URL, mParams);
                                               progress.setMessage(getString(R.string.starting_email));

                                               boolean ok = Cocos2dxActivity.tryEmailAuthor(Cocos2dxCrashHandler.this, true,
                                                                 getString(R.string.crash_preamble)+"\n\n\n\nLog:\n"+log);

                                               progress.dismiss();

                                               if (ok) {

                                                        if (cl.isChecked()) clearState();

                                                        finish();

                                               }

                                     }

                            }.execute();

                            b.postDelayed(new Runnable(){public void run(){

                                     if (task.getStatus() == AsyncTask.Status.FINISHED) return;

                                     // It's probably one of these devices where some fool broke logcat.

                                     progress.dismiss();

                                     task.cancel(true);

                                     new AlertDialog.Builder(Cocos2dxCrashHandler.this)

                                               .setMessage(MessageFormat.format(getString(R.string.get_log_failed), getString(R.string.author_email)))

                                               .setCancelable(true)

                                               .setIcon(android.R.drawable.ic_dialog_alert)

                                               .show();

                            }}, 3000);

                   }});

                   c.setOnClickListener(new View.OnClickListener(){public void onClick(View v){

                            if (cl.isChecked()) clearState();

                            finish();

                   }});

         }

 

         void clearState()

         {

                   SharedPreferences.Editor ed = getSharedPreferences("state", MODE_PRIVATE).edit();

                   ed.clear();

                   ed.commit();

         }

}

 

