package com.example.guclibrarytemp;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatDialogFragment;

import javax.xml.transform.ErrorListener;

public class ErrorDialog extends AppCompatDialogFragment {

    TextView tverror;
    String errormessage;
    Intent gotointent;

    public ErrorDialog(String s) {
        this.errormessage = s;
    }
    public ErrorDialog(String s, Intent nextintent) {
        this.errormessage = s;
        this.gotointent = nextintent;
    }

    @NonNull
    @Override

    public Dialog onCreateDialog(@Nullable Bundle savedInstanceState) {

        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        LayoutInflater inflater = getActivity().getLayoutInflater();
        View view = inflater.inflate(R.layout.layout_dialog, null);
        builder.setView(view).setTitle("Error").setNegativeButton("cancel",
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                }).setPositiveButton("ok", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                if(gotointent != null)
                {
                    startActivity(gotointent);
                }
            }
        });
        tverror = view.findViewById(R.id.Message);
        tverror.setText(errormessage);
        return builder.create();
    }
}
