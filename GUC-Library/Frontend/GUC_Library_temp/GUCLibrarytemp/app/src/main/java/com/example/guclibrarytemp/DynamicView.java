package com.example.guclibrarytemp;

import android.content.Context;
import android.graphics.Color;
import android.text.InputType;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Spinner;
import android.widget.TextView;

public class DynamicView {

    Context ctx;

    public DynamicView(Context ctx) {
        this.ctx = ctx;
    }

/*    public TextView ItemNameView (Context context, String name)
    {
        final ViewGroup.LayoutParams lparams = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        final TextView tv = new TextView(context);
        tv.setLayoutParams(lparams);
        tv.setTextSize(16);
//        tv.setTextColor(Color.rgb(0,0,0));
        tv.setText(" " + name + " ");
        tv.setMaxEms(8);
        return tv;
    }
*/


    public Spinner ItemNameView (Context context, ArrayAdapter at)
    {
        final ViewGroup.LayoutParams lparams = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        final Spinner sp = new Spinner(context);
        sp.setLayoutParams(lparams);
        sp.setAdapter(at);
    //    sp.setTextSize(16);
    //        tv.setTextColor(Color.rgb(0,0,0));
    //    tv.setText(" " + name + " ");
    //    tv.setMaxEms(8);
        return sp;
    }


    public EditText receivedQuantityEditText(Context context)
    {
        final ViewGroup.LayoutParams lparams = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        final EditText editText = new EditText(context);
        int id = 0;
        editText.setId(id);
        editText.setMinEms(2);
        //editText.setTextColor(Color.rgb(0,0,0));
        editText.setInputType(InputType.TYPE_CLASS_NUMBER);
        return editText;
    }

    public TextView priceOfItem(Context context, String text)
    {
        final ViewGroup.LayoutParams lparams = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        final TextView textView = new TextView(context);
        textView.setLayoutParams(lparams);
        textView.setText(text);
        return textView;
    }

}
