package com.example.guclibrarytemp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    private EditText etUsername;
    private EditText etPassword;

    String Username;
    String HostORip = "http://172.18.100.113:3000";

    private RequestQueue requestQueue;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        etUsername = findViewById(R.id.Username);
        etPassword = findViewById(R.id.Password);

        Button BLogin = findViewById(R.id.Login);

        Button Bsignup = findViewById(R.id.signup);
        Bsignup.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent register = new Intent(getApplicationContext(), Register.class);
                register.putExtra("HostOrIp", HostORip);
                startActivity(register);
            }
        });

        BLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String data = "{"+
                        "\"userName\":" + "\"" + etUsername.getText().toString() + "\", "+
                        "\"password\":" + "\"" + etPassword.getText().toString() + "\""+
                        "}";
                Submit(data);
            }
        });
    }

        //            System.out.println("done here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

    private void Submit(String data)
    {
        //System.out.println("heeyyyyy---------------------------------------------------------------------------------------");
        final String savedata= data;
        String URL = HostORip + "/Login/login";

        requestQueue = Volley.newRequestQueue(getApplicationContext());
        StringRequest stringRequest = new StringRequest(Request.Method.POST, URL, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try {
/*                    if(response.contains("wrong username"))
                    {
                        ErrorDialog error = new ErrorDialog("     Wrong username ");
                        error.show(getSupportFragmentManager(), "Error");
                    }
                    else
                    {
                        if(response.contains("wrong password"))
                        {
                            ErrorDialog error = new ErrorDialog("     Wrong password ");
                            error.show(getSupportFragmentManager(), "Error");
                        }
 */
                        if(response.contains("wrong"))
                        {
                            ErrorDialog error = new ErrorDialog("     Wrong username or password");
                            error.show(getSupportFragmentManager(), "Error");
                        }
                        else
                        {
                            JSONObject objres = new JSONObject(response);
                            if(objres.getString("userName").equals(etUsername.getText().toString()))
                            {
                                if(!(objres.getBoolean("isAdmin")))
                                {
                                    Intent i = new Intent(getApplicationContext(), viewwww.class);
                                    i.putExtra("Username", objres.getString("userName"));
                                    i.putExtra("isAdmin", objres.getBoolean("isAdmin"));
                                    i.putExtra("HostOrIp", HostORip);
                                    startActivity(i);
                                }

                                else
                                {
                                    //System.out.println(HostORip);
                                    Intent i = new Intent(getApplicationContext(), AdminViewPage.class);
                                    i.putExtra("Username", objres.getString("userName"));
                                    i.putExtra("isAdmin", objres.getBoolean("isAdmin"));
                                    i.putExtra("HostOrIp", HostORip);
                                    startActivity(i);
                                }
                            }
                        }
/*                    }*/

                } catch (JSONException e) {
                    //System.out.println(e);
                    Intent i = new Intent(getApplicationContext(), Register.class);
                    i.putExtra("HostOrIp", HostORip);
                    ErrorDialog error = new ErrorDialog("     You need to Register ", i);
                    error.show(getSupportFragmentManager(), "Error");
                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {

                 Toast.makeText(getApplicationContext(),"Server Error",Toast.LENGTH_LONG).show();

                //Log.v("VOLLEY", error.toString());
            }
        }) {
            @Override
            public String getBodyContentType() {
                return "application/json; charset=utf-8";
            }

            @Override
            public byte[] getBody() throws AuthFailureError {
                try {
                    return savedata == null ? null : savedata.getBytes("utf-8");
                } catch (UnsupportedEncodingException uee) {
                    //Log.v("Unsupported Encoding while trying to get the bytes", data);
                    return null;
                }
            }

        };
        requestQueue.add(stringRequest);
    }
}
